#!/usr/bin/env python3
"""Synthesizes the sound effects (wav) and the two music loops (ogg).

Usage: tools/make_sounds.py [assets-dir]

Everything is generated from scratch (square / noise / saw oscillators), so
there is no third-party audio in the repository. The music is encoded with
ffmpeg (libvorbis).
"""
import subprocess
import sys
import wave
from pathlib import Path

import numpy as np

RATE = 22050
ASSETS = Path(sys.argv[1] if len(sys.argv) > 1 else
              Path(__file__).resolve().parent.parent
              / "src/games/rtype/assets")
RNG = np.random.default_rng(7)


def t_of(seconds):
    return np.arange(int(seconds * RATE)) / RATE


def square(freq, seconds, duty=0.5):
    t = t_of(seconds)
    phase = (t * freq) % 1.0
    return np.where(phase < duty, 1.0, -1.0)


def saw(freq, seconds):
    return 2.0 * ((t_of(seconds) * freq) % 1.0) - 1.0


def sweep(f0, f1, seconds, shape=square):
    t = t_of(seconds)
    freq = np.linspace(f0, f1, len(t))
    phase = np.cumsum(freq) / RATE
    if shape is square:
        return np.where(phase % 1.0 < 0.5, 1.0, -1.0)
    return 2.0 * (phase % 1.0) - 1.0


def noise(seconds):
    return RNG.uniform(-1.0, 1.0, int(seconds * RATE))


def lowpass(signal, amount):
    out = np.zeros_like(signal)
    acc = 0.0
    for i, x in enumerate(signal):
        acc += amount * (x - acc)
        out[i] = acc
    return out


def decay(signal, power=2.0):
    return signal * np.linspace(1.0, 0.0, len(signal)) ** power


def fade(signal, ms=5):
    n = int(RATE * ms / 1000)
    out = signal.copy()
    out[:n] *= np.linspace(0, 1, n)
    out[-n:] *= np.linspace(1, 0, n)
    return out


def note(name):
    names = {"C": 0, "D": 2, "E": 4, "F": 5, "G": 7, "A": 9, "B": 11}
    semitone = names[name[0]]
    octave = int(name[-1])
    if "#" in name:
        semitone += 1
    return 440.0 * 2 ** ((semitone - 9 + 12 * (octave - 4)) / 12)


def save_wav(path, signal, gain=0.8):
    peak = max(1e-9, np.max(np.abs(signal)))
    data = np.int16(signal / peak * gain * 32767)
    path.parent.mkdir(parents=True, exist_ok=True)
    with wave.open(str(path), "wb") as f:
        f.setnchannels(1)
        f.setsampwidth(2)
        f.setframerate(RATE)
        f.writeframes(data.tobytes())


def shoot():
    return fade(decay(sweep(1100, 260, 0.13), 1.5))


def hit():
    return fade(decay(noise(0.07) * 0.6 + sweep(400, 150, 0.07) * 0.4, 2))


def explosion():
    body = lowpass(noise(0.55), 0.12)
    return fade(decay(body + 0.5 * sweep(140, 40, 0.55, saw), 1.3))


def boss_alert():
    one = sweep(180, 420, 0.5, saw)
    two = sweep(420, 180, 0.5, saw)
    return fade(np.concatenate([one, two, one, two]) * 0.8, 10)


def victory():
    notes = ["C5", "E5", "G5", "C6", "G5", "C6", "E6"]
    lengths = [0.12, 0.12, 0.12, 0.25, 0.12, 0.12, 0.5]
    parts = [fade(decay(square(note(n), s), 0.6)) for n, s in zip(notes, lengths)]
    return np.concatenate(parts)


def defeat():
    notes = ["E4", "D#4", "D4", "C#4"]
    parts = [fade(decay(saw(note(n), 0.4), 0.5)) for n in notes]
    return np.concatenate(parts)


def drums(step_seconds, steps, pattern):
    out = np.zeros(int(step_seconds * RATE) * steps)
    n = int(step_seconds * RATE)
    for i in range(steps):
        kind = pattern[i % len(pattern)]
        if kind == "k":
            hitsig = decay(sweep(150, 45, step_seconds * 0.9, saw), 1.5)
        elif kind == "h":
            hitsig = decay(noise(step_seconds * 0.4), 3) * 0.4
        elif kind == "s":
            hitsig = decay(noise(step_seconds * 0.8), 2) * 0.7
        else:
            continue
        out[i * n:i * n + len(hitsig)] += hitsig[:n]
    return out


def sequence(notes, step_seconds, wave_fn, volume, gate=0.8):
    n = int(step_seconds * RATE)
    out = np.zeros(n * len(notes))
    for i, name in enumerate(notes):
        if name == "-":
            continue
        tone = wave_fn(note(name), step_seconds * gate) * volume
        tone = decay(tone, 0.4)
        out[i * n:i * n + len(tone)] += tone
    return out


def music(bpm, bass, lead, drum_pattern, bars):
    step = 60.0 / bpm / 4.0
    steps = 16 * bars
    mix = sequence(bass * (steps // len(bass)), step, square, 0.5)
    mix += sequence(lead * (steps // len(lead)), step,
                    lambda f, s: square(f, s, 0.25), 0.35)
    mix += drums(step, steps, drum_pattern) * 0.6
    return mix


LEVEL_BASS = (["A2", "-", "A2", "A2", "-", "A2", "-", "A2",
               "F2", "-", "F2", "F2", "-", "F2", "-", "F2",
               "G2", "-", "G2", "G2", "-", "G2", "-", "G2",
               "E2", "-", "E2", "E2", "-", "E2", "E2", "-"])
LEVEL_LEAD = (["A4", "-", "C5", "-", "E5", "-", "C5", "-",
               "F4", "-", "A4", "-", "C5", "-", "A4", "-",
               "G4", "-", "B4", "-", "D5", "-", "B4", "-",
               "E4", "-", "G#4", "-", "B4", "-", "E5", "D5"])
BOSS_BASS = (["D2", "D2", "-", "D2", "D2", "-", "D2", "D#2",
              "D2", "D2", "-", "D2", "D2", "-", "C2", "C#2",
              "B1", "B1", "-", "B1", "B1", "-", "B1", "C2",
              "A1", "A1", "-", "A1", "A1", "A1", "A1", "-"])
BOSS_LEAD = (["D5", "-", "D5", "A4", "-", "D#5", "-", "D5",
              "C5", "-", "C5", "G4", "-", "C#5", "-", "C5",
              "B4", "-", "B4", "F#4", "-", "C5", "-", "B4",
              "A4", "-", "E5", "-", "A4", "-", "D#5", "-"])


def encode_ogg(signal, path):
    peak = max(1e-9, np.max(np.abs(signal)))
    data = np.int16(signal / peak * 0.8 * 32767)
    path.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(["ffmpeg", "-y", "-loglevel", "error", "-f", "s16le",
                    "-ar", str(RATE), "-ac", "1", "-i", "-",
                    "-c:a", "libvorbis", "-q:a", "2", str(path)],
                   input=data.tobytes(), check=True)


def main():
    sfx = ASSETS / "sounds"
    for name, make in {"shoot": shoot, "hit": hit, "explosion": explosion,
                       "boss_alert": boss_alert,
                       "victory": victory, "defeat": defeat}.items():
        save_wav(sfx / f"{name}.wav", make())
        print("wrote", sfx / f"{name}.wav")
    encode_ogg(music(130, LEVEL_BASS, LEVEL_LEAD, "k-h-s-h-k-k-s-h-", 8),
               sfx / "music_level.ogg")
    encode_ogg(music(160, BOSS_BASS, BOSS_LEAD, "k-hks-hkk-hks-hh", 8),
               sfx / "music_boss.ogg")
    print("wrote the music loops")


if __name__ == "__main__":
    main()
