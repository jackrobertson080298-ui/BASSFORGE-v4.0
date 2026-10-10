# BASSFORGE v0.4 — FX Rack (experimental source)

Build with the included GitHub Actions workflow on `windows-2022`.

## Changes
- 7 real-time effects: saturation, low-pass, dynamics, tone EQ, chorus, feedback delay, diffusion-style echo/reverb.
- FX rack tab with enable, mix, amount and reorder buttons; master bypass.
- Preserves all 30 synth patches and the original FM Punisher oscillator settings.
- FX state and effect order saved with host project state.

## Limitations
- DSP is lightweight custom implementation, **not** a full Serum clone.
- The dynamics module is a broadband saturating compressor, **not true multiband compression**.
- Reverb is a simple feedback network, not a studio-grade algorithm.
- Effects are global; factory patches share default FX settings unless you save a DAW preset.
- Onscreen keyboard is visual only; use FL Studio piano roll or MIDI controller.
- The delay and reverb can generate tails but this first build does not guarantee sample-accurate tail reporting.
- Not compiled or tested on Windows in this environment.

**Important:** Back up the working v0.3 VST3 before installing.
