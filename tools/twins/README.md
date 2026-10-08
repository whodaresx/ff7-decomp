# twins

Find reusable code across overlays in this decompilation project.

Run it from the repo root, after a build:

```sh
python3 tools/twins/twins.py
```

It is read-only. It never writes to `src/`, `config/`, or `build/`, so it cannot
disturb anything that has already been matched.

## What it reports

Two kinds of sharing exist in this project and they are not the same thing.

**Duplicated function bodies.** The same function compiled into two different
overlays. This is the copy-paste-and-go case: the solved source is almost
directly reusable. `highway func_800B0754` and `field FieldModelCreatePktsForPart`
are the same 727-instruction function.

**Shared work.** Unmatched functions that are identical to *other unmatched
functions*. Neither side is solved, but they are the same code, so matching one
body covers every copy. This is the largest category in the magic overlays,
where nothing has been solved yet and the "use a solved twin" report is
necessarily empty. 76 such bodies cover 392 functions; one group of 624
instructions is shared by 8 overlays, and one 95-instruction function is shared
by 38. Sorted by payoff (size x copies) rather than size alone, because a small
function in 38 overlays saves more work than a large one in two.

**Call-level reuse.** Overlays that duplicate no code at all but call shared
helpers defined elsewhere. Almost all 280 magic effect overlays are this shape:
thin glue that calls `BattleEffectRegister`, `BattleGetPartPosition`,
`BattleAkaoCommand` and friends. A byte-comparison finds nothing here, so without
this section the tool would report the magic overlays as empty when in fact they
lean on the battle overlay more heavily than anything else does.

All three sections lead with the question a contributor actually has: *what can I
reuse, and where would my work be useful?* It is deliberately not a progress
report. Matching status appears only as a label telling you which side of a pair
is the helper.

## Usage

```sh
python3 tools/twins/twins.py                    # everything
python3 tools/twins/twins.py --only bodies      # unmatched with a solved twin
python3 tools/twins/twins.py --only shared      # unmatched identical to each other
python3 tools/twins/twins.py --only deps        # call-level reuse only
python3 tools/twins/twins.py --only coverage    # what was and was not examined
python3 tools/twins/twins.py --find FUNC        # one function's twin family
```

`--find` works from either end. Asked about an unmatched function it answers
"what can help me?"; asked about a solved one it answers "where else does this
apply?" Both are the same fact read from opposite directions:

```
$ python3 tools/twins/twins.py --find func_800B0754
  This is unmatched. Identical, already-solved copies:
    field     FieldModelCreatePktsForPart      727 insns
    chocobo   func_800AD9D8                    727 insns

$ python3 tools/twins/twins.py --find FieldModelCreatePktsForPart
  This is solved. Anywhere else it appears, the same work applies:
    chocobo   func_800AD9D8                    727 insns
  Still unmatched elsewhere:
    highway   func_800B0754
```

## How it works, and why it is written this way

**Offsets.** Overlays are not flat images, so `file_offset = RAM - K` where `K`
differs per overlay. `K` is derived from each overlay's own `.s` headers, whose
leading comment carries `offset RAMADDR encoding`, then cross-checked against
`vram_start` in `config/us.yaml`. `main` resolves to `vram_start - 0x800`
because it is a real PSX executable with a 0x800-byte header; overlays resolve to
`vram_start`. Any overlay where the two disagree is reported rather than guessed
at, because guessing wrong silently produces nonsense that looks like a result.

**Normalisation.** Two copies of one function sit at different addresses, so
their branch displacements differ, because those encode absolute addresses.
Intra-function branch targets are rewritten to an instruction offset from the
function start. Every other operand, including external calls and global
addresses, is left byte-exact: if two versions call different functions they are
doing different things and should not be reported as twins.

objdump masks the top bit off a 26-bit branch field, so a target of
`RAM 0x800B09C4` prints as `0xb09c4`. Both interpretations are tried when
deciding whether a branch is internal.

**What is skipped.** Only two things, both provably not code: reads outside the
disk image, and all-zero padding. No heuristic plausibility filter is applied.
An earlier version of this tool rejected functions that lacked a stack-frame
prologue and silently lost 72 real nonmatchings, including `AkaoGetNextNote` and
`SysAddCommandToTemp`. Data blobs and switch tables that `nm` reports as `T`
(`D_*`, `jtbl_*`) are excluded from the bodies report, because they are not
decompilable functions.

**Honesty about coverage.** The coverage section reports how many overlays were
read, how many entries were skipped and why, and names any overlay that could
not be examined at all. Silence would be read as "nothing to share", which is
how a tool can be wrong in a way nobody notices.

## Self-test

```sh
python3 tools/twins/selftest.py
```

28 assertions covering the parts that failed silently during development. Each
asserts something verified by hand against the reference binaries, so a
regression surfaces as a failure instead of as a plausible-looking report. Three
injected bugs were confirmed to be caught:

  - branch normalisation not rewriting intra-function targets
  - the `.s` RAM-address pattern rejecting every magic overlay
  - data blobs and switch tables leaking into the function-body report

## Adding an overlay

Nothing here needs updating. The tool reads `config/us.yaml` and the built ELFs.