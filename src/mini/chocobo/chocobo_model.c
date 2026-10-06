//! PSYQ=3.6 CC1=2.6.3
#include "chocobo_private.h"
#include "libetc.h"
#include <libgte.h>
#include <psxsdk/inline_c.h>
#include "sincos.h"

u8* ChocoboModelSetupParts(ChocoboModel* model, u8* buf, s32 arg2) {
    ChocoboModelPart* parts;
    u32 i;

    model->unk20 = buf;
    buf += model->unk2 * sizeof(ChocoboModelPart);
    parts = (ChocoboModelPart*)(model->partsOffset + (u_long)model->data);
    for (i = 0; i < model->nParts; i++) {
        buf = func_800AD9D8(&parts[i], buf, 0, arg2);
    }
    func_800AF9E4(model, model->unk16, 0);
    return buf;
}

u8* func_800AD9D8(ChocoboModelPart* part, u8* nextFree, s32 relocate, s32 modelId) {
    u8* textureFlags;
    u32* polygon;
    u8* cursor;
    u32 buffer;
    u32 i;
    u32 count;
    s32 uOffset;
    s32 vOffset;
    s32 uOffset4;
    s32 vOffset4;
    s32 uOffset8;
    s32 vOffset8;
    u16* texCoords;
    u32* textures;
    POLY_GT4* polyGT4;
    POLY_GT3* polyGT3;
    POLY_FT4* polyFT4;
    POLY_FT3* polyFT3;
    POLY_F3* polyF3;
    POLY_F4* polyF4;
    POLY_G3* polyG3;
    POLY_G4* polyG4;

    textures = (u32*)((u8*)part->verts + part->texturesOffset);
    texCoords = (u16*)((u8*)part->verts + part->texCoordsOffset);
    if (relocate) {
        part->verts = (ChocoboPartVerts*)(part + 1);
    }
    part->packets = nextFree;
    uOffset4 = (modelId % 4) * 64;
    vOffset4 = (modelId / 4) * 32;
    uOffset8 = (modelId % 8) * 32;
    vOffset8 = (modelId / 8) * 32;

    for (buffer = 0; buffer < 2; buffer++) {
        cursor = nextFree;
        textureFlags = (u8*)part->verts + part->textureFlagsOffset;
        if (buffer) {
            cursor += part->packetBufferSize;
        }
        polygon = (u32*)((u8*)part->verts + part->polygonsOffset);
        count = part->polyGT4Count;
        for (i = 0; i < count; i++, cursor += sizeof(POLY_GT4), polygon += 6) {
            u32 uvIndices;
            u32 texture;
            u32 mode;
            s32 paletteOffset;
            u8 flags;

            polyGT4 = (POLY_GT4*)cursor;
            *(u32*)&polyGT4->r0 = polygon[1];
            *(u32*)&polyGT4->r1 = polygon[2];
            *(u32*)&polyGT4->r2 = polygon[3];
            *(u32*)&polyGT4->r3 = polygon[4];
            uvIndices = polygon[5];
            *(u16*)&polyGT4->u0 = texCoords[uvIndices & 0xFF],
            *(u16*)&polyGT4->u1 = texCoords[(uvIndices & 0xFF00) >> 8],
            *(u16*)&polyGT4->u2 = texCoords[(uvIndices & 0xFF0000) >> 16],
            *(u16*)&polyGT4->u3 = texCoords[uvIndices >> 24];
            flags = *textureFlags++;
            texture = textures[flags & 0xF];
            paletteOffset = (texture & 0x3F) == 2 ? 0 : modelId;
            polyGT4->clut = getClut(((texture >> 16) & 0x3F) * 16, ((texture & 0x7FC00000) >> 22) + paletteOffset);
            polyGT4->tpage =
                getTPage((texture & 0xC0) >> 6, flags >> 5, (texture & 0xF00) >> 2, ((texture >> 12) & 0x1) * 256);
            mode = texture & 0x3F;
            if (mode == 0) {
                uOffset = uOffset4;
                vOffset = vOffset4;
            } else if (mode == 1) {
                uOffset = uOffset8;
                vOffset = vOffset8;
            } else {
                vOffset = 0;
                uOffset = 0;
            }
            setPolyGT4(polyGT4);
            polyGT4->u0 += uOffset;
            polyGT4->v0 += vOffset;
            polyGT4->u1 += uOffset;
            polyGT4->v1 += vOffset;
            polyGT4->u2 += uOffset;
            polyGT4->v2 += vOffset;
            polyGT4->u3 += uOffset;
            polyGT4->v3 += vOffset;
            if (flags & 0x10) {
                setcode(polyGT4, 62);
            }
        }
        count = part->polyGT3Count;
        for (i = 0; i < count; i++, cursor += sizeof(POLY_GT3), polygon += 5) {
            u32 uvIndices;
            u32 texture;
            u32 mode;
            s32 paletteOffset;
            u8 flags;

            polyGT3 = (POLY_GT3*)cursor;
            *(u32*)&polyGT3->r0 = polygon[1];
            *(u32*)&polyGT3->r1 = polygon[2];
            *(u32*)&polyGT3->r2 = polygon[3];
            uvIndices = polygon[4];
            *(u16*)&polyGT3->u0 = texCoords[uvIndices & 0xFF],
            *(u16*)&polyGT3->u1 = texCoords[(uvIndices & 0xFF00) >> 8],
            *(u16*)&polyGT3->u2 = texCoords[(uvIndices & 0xFF0000) >> 16];
            flags = *textureFlags++;
            texture = textures[flags & 0xF];
            paletteOffset = (texture & 0x3F) == 2 ? 0 : modelId;
            polyGT3->clut = getClut(((texture >> 16) & 0x3F) * 16, ((texture & 0x7FC00000) >> 22) + paletteOffset);
            polyGT3->tpage =
                getTPage((texture & 0xC0) >> 6, flags >> 5, (texture & 0xF00) >> 2, ((texture >> 12) & 0x1) * 256);
            mode = texture & 0x3F;
            if (mode == 0) {
                uOffset = uOffset4;
                vOffset = vOffset4;
            } else if (mode == 1) {
                uOffset = uOffset8;
                vOffset = vOffset8;
            } else {
                vOffset = 0;
                uOffset = 0;
            }
            setPolyGT3(polyGT3);
            polyGT3->u0 += uOffset;
            polyGT3->v0 += vOffset;
            polyGT3->u1 += uOffset;
            polyGT3->v1 += vOffset;
            polyGT3->u2 += uOffset;
            polyGT3->v2 += vOffset;
            if (flags & 0x10) {
                setcode(polyGT3, 54);
            }
        }
        count = part->polyFT4Count;
        for (i = 0; i < count; i++, cursor += sizeof(POLY_FT4), polygon += 3) {
            u32 uvIndices;
            u32 texture;
            u32 mode;
            s32 paletteOffset;
            u8 flags;

            polyFT4 = (POLY_FT4*)cursor;
            *(u32*)&polyFT4->r0 = polygon[1];
            uvIndices = polygon[2];
            *(u16*)&polyFT4->u0 = texCoords[uvIndices & 0xFF],
            *(u16*)&polyFT4->u1 = texCoords[(uvIndices & 0xFF00) >> 8],
            *(u16*)&polyFT4->u2 = texCoords[(uvIndices & 0xFF0000) >> 16],
            *(u16*)&polyFT4->u3 = texCoords[uvIndices >> 24];
            flags = *textureFlags++;
            texture = textures[flags & 0xF];
            paletteOffset = (texture & 0x3F) == 2 ? 0 : modelId;
            polyFT4->clut = getClut(((texture >> 16) & 0x3F) * 16, ((texture & 0x7FC00000) >> 22) + paletteOffset);
            polyFT4->tpage =
                getTPage((texture & 0xC0) >> 6, flags >> 5, (texture & 0xF00) >> 2, ((texture >> 12) & 0x1) * 256);
            mode = texture & 0x3F;
            if (mode == 0) {
                uOffset = uOffset4;
                vOffset = vOffset4;
            } else if (mode == 1) {
                uOffset = uOffset8;
                vOffset = vOffset8;
            } else {
                vOffset = 0;
                uOffset = 0;
            }
            setPolyFT4(polyFT4);
            polyFT4->u0 += uOffset;
            polyFT4->v0 += vOffset;
            polyFT4->u1 += uOffset;
            polyFT4->v1 += vOffset;
            polyFT4->u2 += uOffset;
            polyFT4->v2 += vOffset;
            polyFT4->u3 += uOffset;
            polyFT4->v3 += vOffset;
            if (flags & 0x10) {
                setcode(polyFT4, 46);
            }
        }
        count = part->polyFT3Count;
        for (i = 0; i < count; i++, cursor += sizeof(POLY_FT3), polygon += 3) {
            u32 uvIndices;
            u32 texture;
            u32 mode;
            s32 paletteOffset;
            u8 flags;

            polyFT3 = (POLY_FT3*)cursor;
            *(u32*)&polyFT3->r0 = polygon[1];
            uvIndices = polygon[2];
            *(u16*)&polyFT3->u0 = texCoords[uvIndices & 0xFF],
            *(u16*)&polyFT3->u1 = texCoords[(uvIndices & 0xFF00) >> 8],
            *(u16*)&polyFT3->u2 = texCoords[(uvIndices & 0xFF0000) >> 16];
            flags = *textureFlags++;
            texture = textures[flags & 0xF];
            paletteOffset = (texture & 0x3F) == 2 ? 0 : modelId;
            polyFT3->clut = getClut(((texture >> 16) & 0x3F) * 16, ((texture & 0x7FC00000) >> 22) + paletteOffset);
            polyFT3->tpage =
                getTPage((texture & 0xC0) >> 6, flags >> 5, (texture & 0xF00) >> 2, ((texture >> 12) & 0x1) * 256);
            mode = texture & 0x3F;
            if (mode == 0) {
                uOffset = uOffset4;
                vOffset = vOffset4;
            } else if (mode == 1) {
                uOffset = uOffset8;
                vOffset = vOffset8;
            } else {
                vOffset = 0;
                uOffset = 0;
            }
            setPolyFT3(polyFT3);
            polyFT3->u0 += uOffset;
            polyFT3->v0 += vOffset;
            polyFT3->u1 += uOffset;
            polyFT3->v1 += vOffset;
            polyFT3->u2 += uOffset;
            polyFT3->v2 += vOffset;
            if (flags & 0x10) {
                setcode(polyFT3, 38);
            }
        }
        count = part->polyF3Count;
        for (i = 0; i < count; i++, cursor += sizeof(POLY_F3), polygon += 2) {
            polyF3 = (POLY_F3*)cursor;
            *(u32*)&polyF3->r0 = polygon[1];
            setPolyF3(polyF3);
        }
        count = part->polyF4Count;
        for (i = 0; i < count; i++, cursor += sizeof(POLY_F4), polygon += 2) {
            polyF4 = (POLY_F4*)cursor;
            *(u32*)&polyF4->r0 = polygon[1];
            setPolyF4(polyF4);
        }
        count = part->polyG3Count;
        for (i = 0; i < count; i++, cursor += sizeof(POLY_G3), polygon += 4) {
            polyG3 = (POLY_G3*)cursor;
            *(u32*)&polyG3->r0 = polygon[1];
            *(u32*)&polyG3->r1 = polygon[2];
            *(u32*)&polyG3->r2 = polygon[3];
            setPolyG3(polyG3);
        }
        count = part->polyG4Count;
        for (i = 0; i < count; i++, cursor += sizeof(POLY_G4), polygon += 5) {
            polyG4 = (POLY_G4*)cursor;
            *(u32*)&polyG4->r0 = polygon[1];
            *(u32*)&polyG4->r1 = polygon[2];
            *(u32*)&polyG4->r2 = polygon[3];
            *(u32*)&polyG4->r3 = polygon[4];
            setPolyG4(polyG4);
        }
    }
    return nextFree + part->packetBufferSize * 2;
}

INCLUDE_ASM("asm/us/mini/chocobo/nonmatchings/chocobo_model", func_800AE534);

INCLUDE_ASM("asm/us/mini/chocobo/nonmatchings/chocobo_model", func_800AE7D4);

INCLUDE_ASM("asm/us/mini/chocobo/nonmatchings/chocobo_model", func_800AF11C);

void func_800AF9E4(ChocoboModel* model, s16 scale, s32 force) {
    MATRIX* scaleMatrix = (MATRIX*)getScratchAddr(0);
    SVECTOR* input = (SVECTOR*)getScratchAddr(8);
    VECTOR* output = (VECTOR*)getScratchAddr(10);
    ChocoboModelPart* parts;
    ChocoboModelAnim* anims;
    ChocoboModelBone* bones;
    u32 count;
    u32 i;

    parts = (ChocoboModelPart*)(model->data + model->partsOffset);
    count = model->nParts;
    for (i = 0; i < count; i++) {
        ChocoboScalePartVerts(&parts[i], scale, force);
    }
    scaleMatrix->m[0][0] = scale;
    scaleMatrix->m[1][1] = scale;
    scaleMatrix->m[2][2] = scale;
    scaleMatrix->m[0][1] = scaleMatrix->m[0][2] = scaleMatrix->m[1][0] = scaleMatrix->m[1][2] = scaleMatrix->m[2][0] =
        scaleMatrix->m[2][1] = scaleMatrix->t[0] = scaleMatrix->t[1] = scaleMatrix->t[2] = 0;
    gte_SetRotMatrix(scaleMatrix);
    gte_SetTransMatrix(scaleMatrix);
    bones = (ChocoboModelBone*)model->data;
    count = (u8)(model->unk2 / 3);
    for (i = 0; i < count; i++) {
        input->vx = bones[i * 3].length;
        input->vy = bones[i * 3 + 1].length;
        input->vz = bones[i * 3 + 2].length;
        gte_ldv0(input);
        gte_rt();
        gte_stlvnl(output);
        bones[i * 3].length = output->vx;
        bones[i * 3 + 1].length = output->vy;
        bones[i * 3 + 2].length = output->vz;
    }
    for (i = count * 3; i < model->unk2; i++) {
        input->vx = bones[i].length;
        gte_ldv0(input);
        gte_rt();
        gte_stlvnl(output);
        bones[i].length = output->vx;
    }
    anims = (ChocoboModelAnim*)(model->data + model->animOffset);
    count = model->nAnims;
    for (i = 0; i < count; i++) {
        func_800AFDBC(&anims[i], scale, force);
    }
}

void ChocoboScalePartVerts(ChocoboModelPart* part, s16 scale, s32 force) {
    MATRIX* m;
    s16* out;
    SVECTOR* verts;
    u32 i;
    u32 n;

    m = (MATRIX*)getScratchAddr(0);
    out = (s16*)(m + 1);
    if ((part->verts->flags & 1) && !force) {
        return;
    }
    m->m[0][0] = scale;
    m->m[1][1] = scale;
    m->m[2][2] = scale;
    m->t[2] = 0;
    m->t[1] = 0;
    m->t[0] = 0;
    m->m[2][1] = 0;
    m->m[2][0] = 0;
    m->m[1][2] = 0;
    m->m[1][0] = 0;
    m->m[0][2] = 0;
    m->m[0][1] = 0;
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
    verts = part->verts->verts;
    n = part->nVerts;
    for (i = 0; i < n; i++) {
        gte_ldv0(&verts[i]);
        gte_rt();
        gte_stlvnl(out);
        verts[i].vx = out[0];
        verts[i].vy = out[2];
        verts[i].vz = out[4];
    }
    part->verts->flags |= 1;
}

void func_800AFDBC(ChocoboModelAnim* anim, s16 scale, s32 force) {
    MATRIX* scaleMatrix = (MATRIX*)getScratchAddr(0);
    // The matrix can be overwritten after it has been loaded into the GTE.
    SVECTOR* input = (SVECTOR*)getScratchAddr(0);
    VECTOR* output = (VECTOR*)getScratchAddr(2);
    u32 frameCount;
    u32 count;
    u32 groups;
    u32 i, j;

    if (!*(u32*)anim->data || force) {
        scaleMatrix->m[0][0] = scale;
        scaleMatrix->m[1][1] = scale;
        scaleMatrix->m[2][2] = scale;
        scaleMatrix->m[0][1] = scaleMatrix->m[0][2] = scaleMatrix->m[1][0] = scaleMatrix->m[1][2] =
            scaleMatrix->m[2][0] = scaleMatrix->m[2][1] = scaleMatrix->t[0] = scaleMatrix->t[1] = scaleMatrix->t[2] = 0;
        gte_SetRotMatrix(scaleMatrix);
        gte_SetTransMatrix(scaleMatrix);
        count = anim->nFrames;
        frameCount = anim->nValues;
        for (i = 0; i < count; i++) {
            s16* trans = (s16*)(anim->data + anim->framesOffset) + i * frameCount;
            groups = frameCount / 3;
            for (j = 0; j < groups; j++) {
                input->vx = trans[j * 3];
                input->vy = trans[j * 3 + 1];
                input->vz = trans[j * 3 + 2];
                gte_ldv0(input);
                gte_rt();
                gte_stlvnl(output);
                trans[j * 3] = output->vx;
                trans[j * 3 + 1] = output->vy;
                trans[j * 3 + 2] = output->vz;
            }
            for (j = groups * 3; j < frameCount; j++) {
                input->vx = trans[j];
                gte_ldv0(input);
                gte_rt();
                gte_stlvnl(output);
                trans[j] = output->vx;
            }
        }
        count = anim->nExtra;
        groups = count / 3;
        for (i = 0; i < groups; i++) {
            s16* statics = (s16*)(anim->data + anim->extraOffset) + i * 3;
            input->vx = statics[0];
            input->vy = statics[1];
            input->vz = statics[2];
            gte_ldv0(input);
            gte_rt();
            gte_stlvnl(output);
            statics[0] = output->vx;
            statics[1] = output->vy;
            statics[2] = output->vz;
        }
        for (i = groups * 3; i < count; i++) {
            s16* statics = (s16*)(anim->data + anim->extraOffset) + i;
            input->vx = *statics;
            gte_ldv0(input);
            gte_rt();
            gte_stlvnl(output);
            *statics = output->vx;
        }
        *(u32*)anim->data = 1;
    }
}

s32 ChocoboModelApplyPartRotation(ChocoboModel* model, u8* data) {
    ChocoboModelPart* parts;
    u32 i;
    u32 n;
    s16 x;
    s16 y;
    s16 z;
    SVECTOR unused;

    n = model->nParts;
    parts = (ChocoboModelPart*)(model->partsOffset + (u_long)model->data);
    x = (data[1] << 8) | data[0];
    y = (data[3] << 8) | data[2];
    z = (data[5] << 8) | data[4];
    *(u_long*)getScratchAddr(0x80) = data[6];
    for (i = 0; i < n; i++) {
        func_800B01B0(&parts[i], x, y, z);
    }
    return 1;
}

INCLUDE_ASM("asm/us/mini/chocobo/nonmatchings/chocobo_model", func_800B01B0);

INCLUDE_ASM("asm/us/mini/chocobo/nonmatchings/chocobo_model", func_800B0E7C);
