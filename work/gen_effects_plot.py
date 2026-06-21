#!/usr/bin/env python3
"""Generate effect plots for tremolo and vibrato."""
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

rate = 44100

# ===========================================================================
# FIGURE 1: Tremolo Effect
# ===========================================================================
f_carrier = 440
A_depth = 0.5
fm_trem = 5
duration = 0.8

t = np.linspace(0, duration, int(rate * duration), endpoint=False)
carrier = np.sin(2 * np.pi * f_carrier * t)
trem_env = (1 + A_depth * np.cos(2 * np.pi * fm_trem * t)) / (1 + A_depth)
trem_signal = carrier * trem_env
ref = carrier

fig1, axes = plt.subplots(2, 1, figsize=(12, 8))

ax = axes[0]
ax.plot(t * 1000, trem_signal, 'b-', linewidth=0.6, label='Tremolo signal')
ax.plot(t * 1000, trem_env, 'r--', linewidth=1.2, alpha=0.7, label='Envelope')
ax.plot(t * 1000, -trem_env, 'r--', linewidth=1.2, alpha=0.7)
ax.set_xlabel('Time (ms)')
ax.set_ylabel('Amplitude')
ax.set_title(f'Tremolo: amplitude modulation with A={A_depth}, f_m={fm_trem} Hz')
ax.legend(fontsize=8, loc='upper right')
ax.grid(True, alpha=0.3)
ax.set_xlim(0, duration * 1000)
ax.set_ylim(-1.3, 1.3)

ax = axes[1]
zoom_start, zoom_end = 0.3, 0.5
mask = (t >= zoom_start) & (t <= zoom_end)
ax.plot(t[mask] * 1000, ref[mask], 'g-', linewidth=1, alpha=0.5, label='Without tremolo')
ax.plot(t[mask] * 1000, trem_signal[mask], 'b-', linewidth=1, label='With tremolo')
ax.fill_between(t[mask] * 1000, ref[mask], trem_signal[mask],
                color='yellow', alpha=0.2, label='Amplitude difference')
ax.set_xlabel('Time (ms)')
ax.set_ylabel('Amplitude')
ax.set_title(f'Zoom: {zoom_start*1000:.0f}–{zoom_end*1000:.0f} ms — effect on amplitude')
ax.legend(fontsize=8, loc='upper right')
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('work/tremolo_effect.png', dpi=150)
print("Generated work/tremolo_effect.png")

# ===========================================================================
# FIGURE 2: Vibrato Effect
# ===========================================================================
I_semitones = 1.0
I_lin = 1 - 2**(-I_semitones / 12.)
fm_vib = 5
f0 = 440
duration_vib = 1.0

t = np.linspace(0, duration_vib, int(rate * duration_vib), endpoint=False)
ref_vib = np.sin(2 * np.pi * f0 * t)

mod_phase = 2 * np.pi * fm_vib * t
step = 1 - I_lin * np.sin(mod_phase)
phase = np.zeros_like(t)
for i in range(1, len(t)):
    phase[i] = phase[i-1] + step[i-1]

phase = 2 * np.pi * f0 * phase / rate
vib_signal = np.sin(phase)

inst_freq = f0 * (1 - I_lin * np.sin(2 * np.pi * fm_vib * t))

fig2, axes = plt.subplots(2, 1, figsize=(12, 8))

ax = axes[0]
zoom_start, zoom_end = 0.3, 0.5
mask = (t >= zoom_start) & (t <= zoom_end)
ax.plot(t[mask] * 1000, ref_vib[mask], 'g-', linewidth=1, alpha=0.5, label='Without vibrato')
ax.plot(t[mask] * 1000, vib_signal[mask], 'b-', linewidth=1, label='With vibrato')
ax.set_xlabel('Time (ms)')
ax.set_ylabel('Amplitude')
ax.set_title(f'Vibrato: frequency modulation with I={I_semitones} semitone(s), f_m={fm_vib} Hz')
ax.legend(fontsize=8, loc='upper right')
ax.grid(True, alpha=0.3)

ax = axes[1]
ax.plot(t * 1000, inst_freq, 'r-', linewidth=1.5)
ax.axhline(y=f0, color='k', linestyle='--', linewidth=0.8, alpha=0.5, label=f'Base frequency {f0} Hz')
ax.set_xlabel('Time (ms)')
ax.set_ylabel('Frequency (Hz)')
ax.set_title('Instantaneous frequency deviation')
ax.legend(fontsize=8)
ax.grid(True, alpha=0.3)
ax.set_xlim(0, duration_vib * 1000)
ax.set_ylim(f0 - 30, f0 + 30)

plt.tight_layout()
plt.savefig('work/vibrato_effect.png', dpi=150)
print("Generated work/vibrato_effect.png")
