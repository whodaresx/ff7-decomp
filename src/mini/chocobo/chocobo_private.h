#ifndef CHOCOBO_PRIVATE_H
#define CHOCOBO_PRIVATE_H
#include <game.h>

#define NUM_CHOCOBO 6

#define ChocoboPrizeTable(name, n, ...)                                                                                \
    struct {                                                                                                           \
        s32 count;                                                                                                     \
        ChocoboPrize prizes[n];                                                                                        \
    } name = {n, {__VA_ARGS__}}

typedef struct {
    /* 0x0 */ u32 flags; // bit 0 is set once the vertices got scaled
    /* 0x4 */ SVECTOR verts[1];
} ChocoboPartVerts;

// The bones are the first thing in the model data, one SVECTOR-sized length each.
typedef struct {
    /* 0x0 */ s16 length;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
} ChocoboModelBone; // size:0x4

typedef struct {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 boneIndex; // unused here - parity with FieldModelPart
    /* 0x02 */ u8 nVerts;
    /* 0x03 */ u8 texCoordCount;
    /* 0x04 */ u8 polyGT4Count;
    /* 0x05 */ u8 polyGT3Count;
    /* 0x06 */ u8 polyFT4Count;
    /* 0x07 */ u8 polyFT3Count;
    /* 0x08 */ u8 polyF3Count;
    /* 0x09 */ u8 polyF4Count;
    /* 0x0A */ u8 polyG3Count;
    /* 0x0B */ u8 polyG4Count;
    /* 0x0C */ u8 textureCount;
    /* 0x0D */ u8 texturedPolygonCount;
    /* 0x0E */ u16 polygonsOffset;
    /* 0x10 */ u16 texCoordsOffset;
    /* 0x12 */ u16 texturesOffset;
    /* 0x14 */ u16 textureFlagsOffset;
    /* 0x16 */ u16 packetBufferSize;
    /* 0x18 */ ChocoboPartVerts* verts; // FieldModelPart calls this `data`
    /* 0x1C */ u8* packets;
} ChocoboModelPart; // size:0x20

typedef struct {
    /* 0x00 */ u16 nValues; // values per frame
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 nFrames;
    /* 0x04 */ u8 nExtra;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u16 framesOffset;
    /* 0x08 */ u16 extraOffset;
    /* 0x0A */ u8 unkA[0x2];
    /* 0x0C */ s32 data; // the first word is set once the values got scaled
} ChocoboModelAnim;      // size:0x10

typedef struct {
    /* 0x00 */ u8 unk0[0x2];
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 nParts;
    /* 0x04 */ u8 nAnims;
    /* 0x05 */ u8 r;
    /* 0x06 */ u8 g;
    /* 0x07 */ u8 b;
    /* 0x08 */ s32 x;
    /* 0x0C */ s32 y;
    /* 0x10 */ s32 z;
    /* 0x14 */ u8 unk14[0x2];
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u16 partsOffset;
    /* 0x1A */ u16 animOffset;
    /* 0x1C */ u8* data;
    /* 0x20 */ u8* unk20;
} ChocoboModel; // size:0x24

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ ChocoboModel* models;
} ChocoboModels;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 speed;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 unk8[0x8];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 unk16[0x12];
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8 unk2E[0xC];
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ u8 unk3C[0xC];
    /* 0x48 */ s16 unk48;
    /* 0x4A */ u8 unk4A[0x2];
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ u8 unk4E[0x4];
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 unk54[0xC];
    /* 0x60 */ s16 unk60;
    /* 0x62 */ u8 unk62[0x2];
    /* 0x64 */ s16 unk64;
    /* 0x66 */ u8 unk66[0x2];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s16 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ u8 unk74[0x8];
    /* 0x7C */ s16 unk7C;
    /* 0x7E */ s16 unk7E;
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 rank;
    /* 0x84 */ u8 unk84[0x2];
    /* 0x86 */ s16 unk86;
    /* 0x88 */ u8 unk88[0x8];
    /* 0x90 */ s16 unk90;
    /* 0x92 */ s16 unk92;
    /* 0x94 */ s16 unk94;
    /* 0x96 */ u8 unk96[0x2];
    /* 0x98 */ s16 unk98;
    /* 0x9A */ s16 unk9A;
    /* 0x9C */ s16 unk9C;
    /* 0x9E */ u8 unk9E[0x2];
    /* 0xA0 */ s16 unkA0;
    /* 0xA2 */ s16 unkA2;
} Chocobo; // size:0xA4

// One edge of the track. A bare SVECTOR would be wrong: the slot after z holds a
// real field on both edges, so this is only the three coordinates.
typedef struct {
    s16 x;
    s16 y;
    s16 z;
} ChocoboTrackEdge; // size:0x6

typedef struct {
    /* 0x00 */ ChocoboTrackEdge p0;
    /* 0x06 */ s16 akaoPreset;      // 1-based idx into ChocoboTrack.akaoPresets, 0 for none
    /* 0x08 */ ChocoboTrackEdge p1;
    /* 0x0E */ s16 nodeStep;        // passed to ChocoboSetNodeStep(); 0x80+ sets step to -1, else +1
    /* 0x10 */ u8 cameraEvent;
    /* 0x11 */ u8 prop; // 1-based idx scenery model on this segment, 0 for none
    /* 0x12 */ u8 cullDistance;
    /* 0x13 */ u8 terrainFlags; // bit 7 is the vis marker
    /* 0x14 */ u8 r;            // bg - used when flags & 2
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 flags; // bit 0: draw track triangles, bit 1: fill background with r/g/b
} ChocoboTrackSegment;   // size:0x18

// Preset AKAO commands fired when a racer crosses a segment with akaoPreset set.
// Copied straight into AkaoCmd and passed to AkaoExec(); see func_800A6E50.
typedef struct {
    /* 0x0 */ u16 opcode; // AkaoOpcode, see include/akao.h
    /* 0x2 */ u16 params[6];
} AkaoCmdPreset; // size:0x10, only 0xE bytes used

// Per-segment lookup table, indexed [segment][field] where field comes from
// Chocobo.unk52. Read one byte at a time; see func_800A500C.
typedef struct {
    /* 0x0 */ u8 field[8];
} ChocoboTrackGuide; // size:0x08

typedef struct {
    /* 0x0 */ ChocoboModels* unk0;
} Unk800B1254;

typedef struct {
    /* 0x0 */ u8 strIndex;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
} ChocoboPrize;

typedef struct {
    /* 0x00000 */ OT_TYPE ot[0x1000];
    /* 0x04000 */ u8 unk4000[0x10];
    /* 0x04010 */ POLY_FT4 prims[2500];
    /* 0x1C6B0 */ POLY_F4 unk1C6B0[NUM_CHOCOBO]; // I think size is NUM_CHOCOBO??
    /* 0x1C740 */ POLY_FT4 unk1C740;
    /* 0x1C768 */ u8 unk1C768[0x18];
    /* 0x1C780 */ POLY_F4 unk1C780;
    /* 0x1C798 */ u8 unk1C798[0x28];
    /* 0x1C7C0 */ POLY_FT4 unk1C7C0;
    /* 0x1C7E8 */ OT_TYPE ot2[4];
    /* 0x1C7F8 */ POLY_G3 polys[250];
    /* 0x1E350 */ POLY_F4 bg;
    /* 0x1E368 */ POLY_FT4 unk1E368[5][3];
    /* 0x1E5C0 */ POLY_FT4 unk1E5C0[5][3];
    /* 0x1E818 */ POLY_FT4 unk1E818[5][3];
    /* 0x1EA70 */ DRAWENV draw;
    /* 0x1EACC */ DISPENV disp;
} GpuBuffer; // size:0x1EAE0

typedef struct {
    /* 0x0 */ u8 index;
    /* 0x1 */ u8 unk1[3];
    /* 0x4 */ GpuBuffer* cur;
} GpuBufferPtr; // size:0x8

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ u32 pressed;
    /* 0x8 */ s32 unk8;
    /* 0xC */ s32 unkC;
} Unk800B7530; // size:0x10

typedef struct {
    /* 0x00 */ s8 type;
    /* 0x01 */ s8 unk1;
    /* 0x02 */ u8 unk2[0x8];
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 unkC[0x8];
} ChocoboTrackEvent; // size:0x14

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ ChocoboTrackEvent event;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 fadeSpeed; // added to D_800B7514 every frame
} Unk800F5040;                // size:0x34

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u16 flags; // bit 1 loops the animation
    /* 0x8 */ s16 step;
    /* 0xA */ s16 frame;
    /* 0xC */ u8 r;
    /* 0xD */ u8 g;
    /* 0xE */ u8 b;
    /* 0xF */ u8 model;
} ChocoboTrackNode; // size:0x10

// The track header is the decompressed track image itself; ChocoboRaceInit casts
// 0x80110000 straight to this, layout below is the file layout. Counts and
// pointers confirmed against both courses (LBA 0x293 = long, 0x33E = short).
typedef struct {
    /* 0x00 */ s32 count;           // number of track segments: 1181 long, 945 short
    /* 0x04 */ s16 lastSprintPoint; // 990 long, 800 short
    /* 0x06 */ u8 unk6[0x2];
    /* 0x08 */ ChocoboTrackSegment* segments; // count x 0x18
    /* 0x0C */ s32 dg4Count;
    /* 0x10 */ void* dg4s;
    /* 0x14 */ s32 dg3Count;
    /* 0x18 */ void* dg3s;
    /* 0x1C */ u16* mapPointers;
    /* 0x20 */ u32* mapTable; // bits 0-15 geo idx, 16-23 type (0=DG3, 1=DG4), bit 31 segment marker
    /* 0x24 */ s16 nTris;     // dome triangle count, always 414
    /* 0x26 */ u8 unk26[0x2];
    /* 0x28 */ struct ChocoboTri* tris; // nTris x 0x24
    /* 0x2C */ s32 spriteCount;
    /* 0x30 */ void* sprites;             // spriteCount x 0x16
    /* 0x34 */ ChocoboTrackGuide* guides; // count x 0x08
    /* 0x38 */ ChocoboTrackEvent* events; // 19 long, 16 short; the count is not stored
    /* 0x3C */ s16 sceneryModelCount;
    /* 0x3E */ u8 unk3E[0x2];
    /* 0x40 */ ChocoboTrackNode* sceneryModels; // sceneryModelCount x 0x10
    /* 0x44 */ AkaoCmdPreset* akaoPresets;      // 21 long, 18 short; indexed by ChocoboTrackSegment.akaoPreset
} ChocoboTrack;                                 // size:0x48

typedef struct ChocoboTri {
    /* 0x00 */ SVECTOR v[3];
    /* 0x18 */ u32 rgb[3]; // CVECTOR with the primitive code slot
} ChocoboTri;              // size:0x24

typedef struct {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u32 unk8;
    /* 0x0C */ ChocoboTrack* track;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s16 unk24;
} Unk800F5078; // size:0x28

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4[0x18];
    /* 0x1C */ s32 unk1C;
} Unk800B7A48; // size:0x20

typedef struct {
    ChocoboPrize* entries;
} ChocoboPrizePtr;

typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 unk6;
    /* 0x7 */ u8 unk7;
} Unk800B7480;

extern SVECTOR D_800A0000;
extern VECTOR D_800A0008;
extern VECTOR D_800A0068;
extern Unk800B1254 D_800B1254;
extern s32 D_800B7478;
extern s32 D_800B733C; // id of the last chocobo
extern ChocoboTrackSegment* D_800B7500;
extern s32 D_800B7514;     // screen fade level, 0 is fully visible and 0x100 is black
extern MATRIX D_800B7544;  // world to screen matrix
extern SVECTOR D_800B7340; // camera eye
extern SVECTOR D_800B7348; // camera target
extern SVECTOR D_800B1348; // eye position the camera moves towards
extern SVECTOR D_800B1350; // target position the camera moves towards
extern s32 D_800B7564[6];
extern Chocobo D_800B75CC[NUM_CHOCOBO + 1];
extern Unk800B7A48 D_800B7A48;
extern GpuBufferPtr D_800B66CC;
extern Unk800B7530 D_800B7530;
extern Unk800F5040 D_800F5040;
extern GpuBuffer* D_800F5074;
extern GpuBuffer D_800B7A68[2];
extern Unk800F5078 D_800F5078;
extern ChocoboModel* D_800F50A0[];
extern s32 D_800B7598;
extern s32 D_800B74F8;
extern s32 D_800B74FC;
extern s32 D_800B747C;
extern s32 D_800B7594;
extern s32 D_800B759C;
extern s32 D_800F5124;
extern s32 D_800F5034; // next free entry of GpuBuffer::prims
extern s32 D_800B745C[3];
extern s16 D_800F5028;
extern s32 D_800B1358;
extern POLY_F4 D_800B14B4;

void ChocoboResetRacerColors(void);
void ChocoboRaceInit(void);
void ChocoboInitMusic(void);
void func_800A18BC(void);
void func_800A1F40(ChocoboModels* models, s32 arg1);
void ChocoboDrawTrackTris(void);
void ChocoboDrawTrackSegments(void);
void func_800A2BD4(s32 start, s32 end);
void func_800A34A8(void);
void ChocoboUpdateRanking(void);
void ChocoboSelectRacer(s32);
void func_800A4888(s32 id);
void ChocoboSelectRacerAtSegment(s32 id, s32 speed, s32 seg);
void func_800A6E50(s32);
void func_800A7CA4(void);
void func_800A8AE8(void);
void func_800A9D94(void);
void func_800AC554(void);
void ChocoboDrawText(const char* str, s32 len, s32 x, s32 y);
void ChocoboDrawFade(void);
u8* ChocoboModelSetupParts(ChocoboModel* model, u8* buf, s32 arg2);
u8* func_800AD9D8(ChocoboModelPart*, u8*, s32, s32);
void func_800AE534(ChocoboModel*);
void func_800AF11C(ChocoboModel*, MATRIX*, s32, s32);
void func_800AF9E4(ChocoboModel* model, s16 scale, s32 force);
void ChocoboScalePartVerts(ChocoboModelPart* part, s16 scale, s32 force);
void func_800AFDBC(ChocoboModelAnim* anim, s16 scale, s32 force);
s32 ChocoboModelApplyPartRotation(ChocoboModel* model, u8* data);
void func_800B01B0(ChocoboModelPart*, s32, s32, s32);

#endif
