import numpy as np
import matplotlib.pyplot as plt

fs = 44100
f0 = 440
t = np.linspace(0, 0.05, int(0.05*fs), endpoint=False)

# FM signals with different parameters
# Reference: pure sine
pure = np.sin(2*np.pi*f0*t)

# FM: N1=1, N2=3, I=2 (clarinete-like)
phase1 = 2*np.pi*f0*t
phase2 = 2*np.pi*3*f0*t
fm_clar = np.sin(phase1 + 2*np.sin(phase2))

# FM: N1=1, N2=1.4, I=3 (campana-like)
phase1b = 2*np.pi*f0*t
phase2b = 2*np.pi*1.4*f0*t
fm_bell = np.sin(phase1b + 3*np.sin(phase2b))

fig, axes = plt.subplots(3, 1, figsize=(12, 8), sharex=True)

axes[0].plot(t*1000, pure, label='Seno puro (A4 = 440 Hz)', color='gray', alpha=0.7)
axes[0].plot(t*1000, fm_clar, label='FM N1=1, N2=3, I=2 (clarinete)', color='#2E86AB')
axes[0].set_ylabel('Amplitud')
axes[0].legend(loc='upper right')
axes[0].grid(True, alpha=0.3)

axes[1].plot(t*1000, pure, label='Seno puro (A4 = 440 Hz)', color='gray', alpha=0.7)
axes[1].plot(t*1000, fm_bell, label='FM N1=1, N2=1.4, I=3 (campana)', color='#A23B72')
axes[1].set_ylabel('Amplitud')
axes[1].legend(loc='upper right')
axes[1].grid(True, alpha=0.3)

# FM vibrato: N1=1, N2=1, I=0.5, fm=5 Hz (sub-audio modulation)
t_long = np.linspace(0, 1.0, fs, endpoint=False)
f0_long = 440
fm_mod = 5
phase_carrier = 2*np.pi*f0_long*t_long
phase_mod = 2*np.pi*fm_mod*t_long
I_val = 0.5
fm_vib = np.sin(phase_carrier + I_val*np.sin(phase_mod))

inst_freq = f0_long + f0_long*I_val*fm_mod/f0_long*np.cos(2*np.pi*fm_mod*t_long)

axes[2].plot(t_long[:2000]*1000, fm_vib[:2000], label='FM vibrato (N1=N2=1, I=0.5, fm=5 Hz)', color='#F18F01')
axes[2].set_xlabel('Tiempo (ms)')
axes[2].set_ylabel('Amplitud')
axes[2].legend(loc='upper right')
axes[2].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('work/doremi/fm_signals.png', dpi=150)
print("Generated work/doremi/fm_signals.png")
