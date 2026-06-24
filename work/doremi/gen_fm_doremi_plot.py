import numpy as np
import matplotlib.pyplot as plt
import struct

def read_wav(filename):
    with open(filename, 'rb') as f:
        data = f.read()
    # Find data chunk
    idx = data.find(b'data')
    if idx < 0:
        return None
    # Skip RIFF header and fmt chunk
    data_start = idx + 8
    raw = data[data_start:]
    # 16-bit samples
    samples = np.frombuffer(raw, dtype=np.int16).astype(np.float32) / 32768.0
    return samples

clar = read_wav('work/doremi/clarinete.wav')
bell = read_wav('work/doremi/campana.wav')

if clar is None or bell is None:
    print("Cannot read WAV files")
    exit()

fs = 44100
t = np.arange(len(clar)) / fs

fig, axes = plt.subplots(2, 1, figsize=(12, 6), sharex=True)

axes[0].plot(t, clar, color='#2E86AB', linewidth=0.5)
axes[0].set_ylabel('Amplitud')
axes[0].set_title('Escala diatónica — FM clarinete (N1=1, N2=3, I=2)')
axes[0].set_xlim([0, len(clar)/fs])
axes[0].grid(True, alpha=0.3)

axes[1].plot(t, bell, color='#A23B72', linewidth=0.5)
axes[1].set_ylabel('Amplitud')
axes[1].set_title('Escala diatónica — FM campana (N1=1, N2=1.4, I=3)')
axes[1].set_xlabel('Tiempo (s)')
axes[1].set_xlim([0, len(bell)/fs])
axes[1].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('work/doremi/fm_doremi_scales.png', dpi=150)
print("Generated work/doremi/fm_doremi_scales.png")
