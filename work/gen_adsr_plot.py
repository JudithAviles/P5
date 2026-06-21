#!/usr/bin/env python3
"""Generate ADSR envelope plots from actual synth parameters."""
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

rate = 44100

def adsr_envelope(A, D, S, R, duration, note_on_end, sample_rate=rate):
    n_attack = int(0.5 + A * sample_rate)
    n_decay = int(0.5 + D * sample_rate)
    n_pressed = n_attack + n_decay
    n_release = int(0.5 + R * sample_rate)
    n_total = int(duration * sample_rate)
    
    env = np.zeros(n_total)
    
    # Attack
    if n_attack > 0:
        env[:n_attack] = np.linspace(0, 1, n_attack)
    # Decay
    if n_decay > 0:
        env[n_attack:n_pressed] = np.linspace(1, S, n_decay)
    # Sustain
    env[n_pressed:note_on_end] = S
    
    # Release
    release_start = note_on_end
    release_end = min(release_start + n_release, n_total)
    if release_end > release_start:
        env[release_start:release_end] = S * np.linspace(1, 0, release_end - release_start)
    
    return env, np.arange(n_total) / sample_rate

def plot_adsr(ax, t, env, title, labels=True):
    ax.plot(t, env, 'b-', linewidth=1.5)
    ax.set_xlabel('Time (s)')
    ax.set_ylabel('Amplitude')
    ax.set_title(title)
    ax.set_xlim(0, t[-1])
    ax.set_ylim(-0.05, 1.05)
    ax.grid(True, alpha=0.3)
    if labels:
        # Find key points
        max_idx = np.argmax(env)
        ax.annotate('Attack', xy=(t[max_idx*3//10], 0.95),
                    xytext=(t[max_idx*3//10], 1.1),
                    arrowprops=dict(arrowstyle='->'), ha='center')
        ax.annotate('Decay', xy=(t[min(len(t)-1, max_idx*2)], env[min(len(env)-1, max_idx*2)]),
                    xytext=(t[min(len(t)-1, max_idx*2)], 0.5),
                    arrowprops=dict(arrowstyle='->'), ha='center')
        # Find release point
        for i in range(1, len(env)):
            if env[i] < env[i-1] * 0.9 and i > len(env)//2:
                ax.annotate('Release', xy=(t[i], env[i]),
                            xytext=(t[i], 0.5),
                            arrowprops=dict(arrowstyle='->'), ha='center')
                break

# Figure 1: Generic ADSR (Clarinet)
fig, axes = plt.subplots(2, 2, figsize=(12, 8))

# Generic ADSR
t_env, t = adsr_envelope(0.1, 0.3, 0.5, 0.4, 1.5, int(0.8 * rate))
plot_adsr(axes[0,0], t, t_env, 'Generic ADSR (InstrumentClar)\nADSR_A=0.1  ADSR_D=0.3  ADSR_S=0.5  ADSR_R=0.4')
axes[0,0].axvline(x=0.8, color='r', linestyle='--', alpha=0.5, label='NoteOff')
axes[0,0].legend(fontsize=8)

# Percussive - held to extinction
t_env, t = adsr_envelope(0.001, 0.5, 0.0, 1.0, 3.0, int(3.0 * rate))
plot_adsr(axes[0,1], t, t_env, 'Percussive - Held to Extinction\nADSR_A=0.001  ADSR_D=0.5  ADSR_S=0  ADSR_R=1.0\n(Note held until sound naturally dies)')
axes[0,1].axvline(x=0.5, color='g', linestyle=':', alpha=0.5, label='Decay ends')
axes[0,1].legend(fontsize=8)

# Percussive - early release
t_env, t = adsr_envelope(0.001, 0.5, 0.0, 1.0, 1.5, int(0.2 * rate))
plot_adsr(axes[1,0], t, t_env, 'Percussive - Early Release\nSame parameters, NoteOff at 0.2s (during decay)')
axes[1,0].axvline(x=0.2, color='r', linestyle='--', alpha=0.5, label='NoteOff\n(release triggered)')
axes[1,0].legend(fontsize=8)

# Flat/Sustained (Plano)
t_env, t = adsr_envelope(0.01, 0.0, 0.8, 0.05, 1.0, int(0.8 * rate))
plot_adsr(axes[1,1], t, t_env, 'Flat/Sustained (InstrumentPlano)\nADSR_A=0.01  ADSR_D=0  ADSR_S=0.8  ADSR_R=0.05')
axes[1,1].axvline(x=0.8, color='r', linestyle='--', alpha=0.5, label='NoteOff')
axes[1,1].legend(fontsize=8)

plt.tight_layout()
plt.savefig('adsr_envelopes.png', dpi=150)
print("Generated adsr_envelopes.png")

# Figure 2: Seno signal detail
fig2, ax2 = plt.subplots(figsize=(10, 5))
# Generate a few cycles of Seno
freq = 440  # A4
t_vals = np.linspace(0, 0.005, 500)
signal = np.sin(2 * np.pi * freq * t_vals)

# Wavetable points (N=40)
N = 40
tbl_t = np.linspace(0, 0.005, N)
tbl_sig = np.sin(2 * np.pi * freq * tbl_t / 440 * 44100 / N)

# Actually let me just use the same time axis for wavetable samples
tbl_idx = np.arange(N)
tbl_time = tbl_idx / (44100 / freq * N) * 1/freq

ax2.plot(t_vals * 1000, signal, 'b-', linewidth=1, label='Generated signal (44100 Hz)')
ax2.plot(tbl_time * 1000, np.sin(2 * np.pi * tbl_idx / N), 'ro', markersize=5, label=f'Wavetable samples (N={N})')

# Interpolated points
t_interp = np.linspace(0, 0.005, 200)
ax2.plot(t_interp * 1000, np.sin(2 * np.pi * 440 * t_interp), 'g--', alpha=0.5, linewidth=1, label='Ideal sine')

ax2.set_xlabel('Time (ms)')
ax2.set_ylabel('Amplitude')
ax2.set_title('InstrumentSeno: Wavetable with Linear Interpolation\nN=40, Frequency=440Hz (A4)')
ax2.legend(fontsize=8)
ax2.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('seno_wavetable.png', dpi=150)
print("Generated seno_wavetable.png")
