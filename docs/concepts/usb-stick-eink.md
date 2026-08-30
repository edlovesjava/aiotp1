# Concept: e-paper USB stick with scroll-and-press UI

**Status:** concept / feasibility sketch. No hardware committed.

A USB-stick-sized device with a bistable e-paper display and a single
scroll+press control, so that it can tell you things about itself while it is
*unplugged* — how full it is, what time it is, how much power it drew, what
protocol it will speak when you plug it in next.

The whole idea rests on one property: **e-paper holds its image at zero power.**
A flash drive that shows "72% full · 14:32 · 61% batt" on a shelf for a year is
not doing anything expensive. That is the feature.

---

## 1. Verdict up front

| Ask | Feasible? | Notes |
|---|---|---|
| Stick form factor + e-paper | Yes, with caveats | Slim thumb-drive size caps you at a 1.02"/1.54" panel. A useful 2.13" panel makes it a fat stick / key-card. |
| Scroll + press UI | Yes | Prefer capacitive strip + one real dome switch over a mechanical encoder. |
| **Contents view / "how full?"** | Yes — **but it dictates the architecture** | Only works if the MCU owns the filesystem. See §4. This is the highest-cost requirement in the whole list. |
| Power / battery status | Yes, easy | |
| Date/time | Yes, easy | Needs a real RTC and a host-side channel to set it. |
| Alarm | Yes, easy | Piezo or LRA; energy cost is noise. |
| Protocol screen (MSC/MTP/HID/BLE) | Yes, and it's the best feature | Pick your USB personality *before* plugging into a machine you don't trust. |
| Read V / A / power | Yes — **but only pass-through is interesting** | Needs a second connector. See §6. |
| Read audio/video | Partly | Metadata + audio: yes. Video playback on e-ink: no. Camera: don't. See §7. |

The honest summary: everything on the list is buildable, but two items —
"contents view" and "read V/A" — each quietly change the product. The first
forces MCU-mediated storage (≈10× slower than a real flash drive). The second
forces a second connector (the stick becomes an inline dongle).

---

## 2. Form factor

E-paper panel outlines that matter:

| Panel | Resolution | Module outline | Implied body |
|---|---|---|---|
| 1.02" | 128×80 | 33.0 × 25.0 mm | 68 × 24 × 8 mm — a real thumb drive |
| 1.54" | 200×200 | 31.8 × 31.8 mm | 70 × 36 × 8 mm — square-ish, awkward |
| **2.13"** | **250×122** | **59.2 × 29.2 mm** | **85 × 34 × 9.5 mm — key-card / fat stick** |
| 2.9" | 296×128 | 79.0 × 36.7 mm | body is now all display; not a stick |

Two SKUs worth prototyping:

- **Slim** — 68 × 24 × 8 mm, 1.02" 128×80. Genuinely pocketable, genuinely a
  flash drive. The display fits a clock, a fill bar, and one line of text. No
  file browsing.
- **Wide** — 85 × 34 × 9.5 mm, 2.13" 250×122. Roughly a Ledger Nano X in a
  longer body. Five text rows at 8pt, or a fill gauge plus four rows. This is
  where the scroll UI actually earns its place.

Thickness budget for Wide: EPD glass 1.0 + FPC bend 0.7 + PCB 0.8 + LiPo 4.0 +
2× wall 1.0 + tolerance ≈ 9.5 mm. Tight but standard.

**Connector: USB-C, not USB-A.** 8.3 × 2.5 mm vs 12 × 4.5 mm, and you need C's
role-swap and CC lines for §6 anyway.

### The mechanical failure mode nobody plans for

A stick plugged into a laptop is a **lever arm**, and an EPD is **glass**. Bump
a plugged-in stick and the panel cracks, not the connector. Mitigations, pick
one: recess the plug so the body sits flush against the host, put the plug on a
short captive flex/cable, use a rotating/retractable plug, or bond the panel to
a stiffener and accept the extra 0.5 mm. Do not skip this.

---

## 3. Input: scroll + press

| Option | Z-height | Feel | Sealing | Verdict |
|---|---|---|---|---|
| Alps EVQWGD001 scroll encoder | 5.7 mm | Excellent, detented | Dust ingress path | Great for the prototype |
| Capacitive strip (PCB pads + MCU touch peripheral) | 0 mm | No detents, needs haptic feedback | Fully sealed | Best for production |
| 3× tactile domes (up/down/select) | 0.8 mm | Fine, unglamorous | Good | Safest fallback |

**Recommendation: capacitive strip for scroll + one real tactile dome for
press.** The dome must be physical, for two reasons: it can wake the device from
deep sleep with zero standby cost, and it gives you a **hardware presence
check** — a YubiKey-style "a human is physically here" gate on unlock, wipe, and
mode change. That is not achievable with a capacitive pad you can't trust.

E-ink UI rules that follow from the hardware:

- Full refresh = 1–2 s of black/white flashing. Use it on wake and once every
  ~20 partial refreshes to clear ghosting. Never mid-scroll.
- Partial refresh ≈ 300 ms. A scroll step must repaint **only the two rows that
  changed**, not the screen.
- No smooth scrolling, no animation, no progress spinners. Discrete rows,
  discrete steps.
- Pre-render the idle screen so wake-to-readable is one partial refresh.

---

## 4. Storage and "how full?" — the load-bearing decision

This is the requirement that picks the architecture.

**Option A — off-the-shelf UFD controller (real flash-drive speed).** The
controller owns the NAND, the host owns the filesystem, and your MCU can see
neither. Free space is **unknowable** offline. Rejected.

**Option B — MCU owns the storage.** MCU + eMMC (or microSD/QSPI NAND), MCU
presents the volume over USB. Now the MCU can read the FAT/exFAT allocation
tables, compute free space, and walk directories for a contents view. This is
the only architecture where the display can tell the truth. **Chosen.**

The cost is throughput. An MCU with USB High-Speed and DMA to eMMC does
**20–35 MB/s**. A real USB 3.2 stick does 400+. You are trading roughly 10× of
write speed for the screen. That is acceptable if this is a *documents, keys,
configs, logs, firmware images* stick and unacceptable if it is a *4K dailies*
stick. Position it as the former.

### Coherency hazard (important)

USB Mass Storage (MSC) is **block-level**: while mounted, the host owns the
filesystem and caches metadata. If the device parses the FAT at the same time it
will show stale or torn state, and if it ever *writes* it corrupts the volume.

Two ways out:

1. **MTP as the primary protocol.** File-level, device owns the filesystem, host
   asks politely. The contents view is then trivially correct, and you get
   per-file operations on the device. This is what cameras and phones do.
   Downside: native on Windows and Linux (gvfs/libmtp), poor on macOS without
   third-party software.
2. **MSC with a discipline:** device parses the FS only when *not* mounted (no
   SCSI activity + host has sent SYNCHRONIZE CACHE / bus is suspended), and
   never writes to the volume itself. Correct most of the time, stale
   occasionally.

**Do both and let the wheel choose** — which is exactly what the "protocols"
screen in §5 is for. MSC for "plug into any machine, including a printer";
MTP for "I want the screen to be right."

### What MCU-owned storage buys you for free

Because the MCU is in the data path, you get the features that actually justify
the price of the device:

- **On-device encryption with a PIN entered by scroll+press.** The host never
  sees the passphrase, and there is no software to install. This is the real
  product.
- Hardware write-protect toggle (a physical mode, not a filesystem flag).
- Per-directory lock / selective mount: only expose `/public` unless unlocked.
- Tamper-evident access log written to a partition the host cannot see.
- Auto-lock on unplug, with the screen showing "LOCKED" while it sits in a
  drawer.

---

## 5. The screens

```
HOME      [████████░░] 72%  ·  118 GB free  ·  14:32  ·  [##  ] 61%
FILES     browse; sizes, counts, last-modified; per-item lock
POWER     battery %, mV, charge cycles; (pass-through: V / A / W / mAh)
CLOCK     date, time, timezone, drift since last sync
ALARM     wake times; "eject reminder"; "sync overdue"
PROTOCOL  MSC | MTP | HID-only | CDC | BLE | OFF   ← pick before plugging in
SECURITY  lock / unlock / change PIN / wipe (press-and-hold confirm)
ABOUT     firmware, serial, health, wear level
```

"PROTOCOL" is the sleeper feature. Selecting **HID-only** or **OFF** before
plugging into a kiosk, a rental car, or a conference charger means the device
physically cannot present a storage endpoint. That is a data-blocker with a
display, and it's a stronger story than "flash drive with a screen."

---

## 6. "Can it read A, V, I data?" — electrical reading

Reading it as volts / amps / current: **yes, and the sensing is trivial.**

- VBUS via resistor divider into the MCU ADC.
- Current via high-side shunt (10–50 mΩ) + **INA228** or INA226 on I²C. The
  INA228 integrates charge and energy in hardware, so the stick can accumulate
  mAh/mWh while the MCU sleeps and just wake to repaint.
- PD contract decoding via an **FUSB302** on the CC lines: read the source
  capabilities and the negotiated PDO, show "9 V / 3 A · 27 W · PPS".

**The catch: a stick is a sink.** Wired as a normal flash drive it can only
measure *its own* consumption, which is a rounding error and not interesting.
To measure what a *phone or laptop* is drawing you must be in the path — which
means **a second connector**: USB-C male on one end, USB-C female on the other.

That is still stick-shaped (~85 mm), and it makes the device considerably more
compelling: **storage + power meter + PD sniffer + data blocker + clock**, with
an e-ink readout that survives being unplugged so you can read the session total
after the fact.

Constraints on the pass-through:

- Cap at **3 A / 60 W** unless you commit to real copper, thermal design, and an
  e-marker. 5 A / 100 W+ in a 9.5 mm body is a different project.
- Pass **USB 2.0 data only**. Transparent USB 3.x pass-through at 5–10 Gbps
  needs redrivers and controlled impedance; it will double the BOM and the
  layout effort for a device whose own storage tops out at 35 MB/s anyway.
- Handle CC/Ra/Rd correctly or you will break charging for the device behind you.

---

## 7. "A, V, I" — audio/video reading

If the question meant audio/video/image instead:

- **Reading A/V *files* (metadata):** yes, easily. Parse ID3 / MP4 atoms / EXIF
  on the MCU and show track names, durations, shot dates. A 250×122 1-bit panel
  can render a **dithered thumbnail or poster frame** — Atkinson dithering on
  e-ink looks genuinely good.
- **Audio playback:** yes. Enumerate as USB Audio Class to a host, or add a
  small DAC and become a standalone player with an e-ink transport UI.
- **Video playback:** **no.** E-ink partial refresh is ~3 fps at best, mono,
  with ghosting. Show a poster frame; do not promise motion.
- **Audio capture:** yes, and it's a good fit. A MEMS I²S mic (e.g. ICS-43434)
  is ~3.5 × 2.6 mm. A voice-memo stick with an e-ink level meter and a real
  press-to-record button is a coherent product.
- **Camera:** technically possible (OV5640 + ~5–6 mm lens stack), but it blows
  the thickness budget, forces a bigger MCU, and — more to the point — **a
  camera concealed in a flash-drive body is legally restricted in many
  jurisdictions.** If imaging is wanted, make the lens obvious and external.

Third reading, if this is meant as an AIoT sensor node: exposing I²C / 1-Wire /
ADC on pogo pads turns it into a **datalogger with a display** — log to internal
flash while unplugged, then present the log as a mountable volume. That is a
small firmware addition on top of everything above and probably the cheapest way
to make the thing "AIoT" rather than "a gadget."

---

## 8. Power budget

Assume a 401030 LiPo pouch: 4 × 10 × 30 mm, **100 mAh** (370 mWh).

| Activity | Draw | Cost |
|---|---|---|
| Deep sleep (MCU + RTC + PMIC quiescent + leakage) | ~4 µA | 0.1 mAh/day |
| EPD partial refresh (0.3 s @ ~8 mA) | — | **0.67 µAh** |
| EPD full refresh (2 s @ ~10 mA) | — | **5.6 µAh** |
| Interactive session (30 s awake + ~20 partial refreshes) | ~10 mA | **~0.1 mAh** |
| Alarm buzz (5 s @ 15 mA) | — | 0.02 mAh |

Which gives:

- **~1000 interactive sessions per charge.** That is the number that matters.
- **Minute-resolution always-on clock: ~3 months** (1440 refreshes/day ≈
  1 mAh/day).
- **Hour-resolution clock: standby-limited, >1 year** — at which point LiPo
  self-discharge (2–3 %/month) is the binding constraint, not the circuit.

**Design rule that falls out of this:** don't run a minute clock unconditionally.
Repaint every minute for ~10 minutes after the last touch, then drop to hourly.
The user gets a live clock when they're looking at it and a year of shelf life
when they aren't.

RTC: **RV-3028-C7** — 45 nA, ±1 ppm (±32 s/year). A bare 32.768 kHz crystal at
±20 ppm drifts a minute a year and will make the clock feature feel broken.

Charging: charge from VBUS when plugged in. MCP73831 for the cheap path,
BQ25150 if you want a fuel gauge and proper termination in one part. Note that
a LiPo in a shipped product means a protection IC and **UN38.3 transport
testing** — a real schedule item, not a footnote.

**Temperature:** most EPDs are specified 0–40 °C and refresh badly below
freezing. A stick that lives in a car in a Michigan winter is out of spec. Say
so on the box.

---

## 9. Parts sketch

| Block | Part | Why |
|---|---|---|
| MCU | **STM32U5** (U59x for USB HS + PHY) | USB High-Speed, ultra-low sleep, TrustZone + AES, OCTOSPI + SDMMC. The one part that satisfies both "fast enough for storage" and "µA in sleep". |
| MCU (alt) | ESP32-S3 | If Wi-Fi sync matters more than shelf life; costs you ~7 µA→ higher sleep and a bigger cell. |
| MCU (proto) | RP2350 | Cheapest path to a working demo; USB Full-Speed only, so ~1 MB/s. Fine for the mockup, not the product. |
| Display | 2.13" 250×122 EPD (SSD1680-class) | Best size/aspect match for a stick. |
| Storage | eMMC 64–256 GB, or microSD | eMMC for the sealed product, microSD for the prototype. |
| Power meter | **INA228** + 20 mΩ shunt | 20-bit, hardware charge/energy accumulation. |
| PD sniffer | FUSB302B | Decode source caps and the negotiated contract. |
| RTC | RV-3028-C7 | 45 nA, ±1 ppm. |
| Charger | BQ25150 | Charger + fuel gauge + load switch. |
| Input | Capacitive strip (MCU peripheral) + 1 tactile dome | Sealed scroll, trustworthy press. |
| Alarm | 9×9×3 mm magnetic buzzer, or LRA | Either fits; LRA is politer. |
| Battery | 401030 LiPo, 100 mAh | Fits the 4 mm slot. |

**Weekend prototype:** Waveshare 2.13" EPD + RP2350 (TinyUSB has a working MSC
example) + INA228 breakout + Alps EVQWGD001 encoder, on a breadboard. It will
prove out the UI, the refresh feel, and the FAT-parsing logic — which are the
three things most likely to disappoint — before anyone lays out a board.

---

## 10. Risks

1. **UI latency.** 300 ms per scroll step is the floor. If the menu design
   fights that, the device feels broken. Prototype the UI first.
2. **BOM/positioning.** Realistically $25–40 BOM → $80–120 retail. That is
   Ledger pricing, not SanDisk pricing. Sell the screen and the security, not
   the gigabytes.
3. **Throughput.** 20–35 MB/s will be the top review complaint if the marketing
   says "flash drive."
4. **MSC coherency** (§4). MTP fixes it and macOS punishes it.
5. **Mechanical lever arm** (§2). Cracked panels in the field.
6. **Battery compliance.** Protection IC, UN38.3, and airline/shipping rules.
7. **EPD temperature range** (§8).
8. If the camera variant is pursued: legality and consent (§7).

---

## 11. Open question

**"A, V, I" is ambiguous and the answer changes the BOM:**

- **Volts/amps/current** → §6: add a second USB-C connector, INA228, FUSB302.
  The device becomes an inline meter that also stores files.
- **Audio/video/images** → §7: add an I²S mic and/or a DAC. The device becomes a
  recorder/player that also stores files.
- **Analog sensor / instrumentation data** → §7 tail: add pogo pads and an ADC
  front end. The device becomes a datalogger that also stores files.

All three are compatible with everything in §§1–5, and none of them are
compatible with each other in a 9.5 mm body. Pick one for v1.
