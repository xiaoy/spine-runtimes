/******************************************************************************
 * Spine Runtimes Software License
 * Version 2.1
 * 
 * Copyright (c) 2013, Esoteric Software
 * All rights reserved.
 * 
 * You are granted a perpetual, non-exclusive, non-sublicensable and
 * non-transferable license to install, execute and perform the Spine Runtimes
 * Software (the "Software") solely for internal use. Without the written
 * permission of Esoteric Software (typically granted by licensing Spine), you
 * may not (a) modify, translate, adapt or otherwise create derivative works,
 * improvements of the Software or develop new applications using the Software
 * or (b) remove, delete, alter or obscure any trademarks or any copyright,
 * trademark, patent or other intellectual property or proprietary rights
 * notices on or in the Software, including any copy thereof. Redistributions
 * in binary or source form must include this license and terms.
 * 
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ESOTERIC SOFTARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include <spine/spine-cocos2dx.h>
#include <spine/extension.h>

USING_NS_CC;

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path) {
	Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(path);
	TextureAtlas* textureAtlas = TextureAtlas::createWithTexture(texture, 128);
	textureAtlas->retain();
	self->rendererObject = textureAtlas;
	self->width = texture->getPixelsWide();
	self->height = texture->getPixelsHigh();
}

void _spAtlasPage_disposeTexture (spAtlasPage* self) {
	((TextureAtlas*)self->rendererObject)->release();
}

char* _spUtil_readFile (const char* path, int* length) {
	Data data = FileUtils::getInstance()->getDataFromFile(
			FileUtils::getInstance()->fullPathForFilename(path).c_str());
	*length = data.getSize();
	char* bytes = MALLOC(char, *length);
	memcpy(bytes, data.getBytes(), *length);
	return bytes;
}

spTrackEntry* _spAnimationState_createTrackEntry (spAnimationState* self) {
	return _spTrackEntry_create();
}

void _spAnimationState_disposeTrackEntry (spAnimationState* self, spTrackEntry* entry) {
	if (entry->rendererObject) FREE(entry->rendererObject);
	_spTrackEntry_dispose(entry);
}

/**/

namespace spine {

void spRegionAttachment_updateQuad (spRegionAttachment* self, spSlot* slot, V3F_C4B_T2F_Quad* quad, bool premultipliedAlpha) {
	float vertices[8];
	spRegionAttachment_computeWorldVertices(self, slot->skeleton->x, slot->skeleton->y, slot->bone, vertices);

	GLubyte r = slot->skeleton->r * slot->r * 255;
	GLubyte g = slot->skeleton->g * slot->g * 255;
	GLubyte b = slot->skeleton->b * slot->b * 255;
	float normalizedAlpha = slot->skeleton->a * slot->a;
	if (premultipliedAlpha) {
		r *= normalizedAlpha;
		g *= normalizedAlpha;
		b *= normalizedAlpha;
	}
	GLubyte a = normalizedAlpha * 255;
	quad->bl.colors.r = r;
	quad->bl.colors.g = g;
	quad->bl.colors.b = b;
	quad->bl.colors.a = a;
	quad->tl.colors.r = r;
	quad->tl.colors.g = g;
	quad->tl.colors.b = b;
	quad->tl.colors.a = a;
	quad->tr.colors.r = r;
	quad->tr.colors.g = g;
	quad->tr.colors.b = b;
	quad->tr.colors.a = a;
	quad->br.colors.r = r;
	quad->br.colors.g = g;
	quad->br.colors.b = b;
	quad->br.colors.a = a;

	quad->bl.vertices.x = vertices[SP_VERTEX_X1];
	quad->bl.vertices.y = vertices[SP_VERTEX_Y1];
	quad->tl.vertices.x = vertices[SP_VERTEX_X2];
	quad->tl.vertices.y = vertices[SP_VERTEX_Y2];
	quad->tr.vertices.x = vertices[SP_VERTEX_X3];
	quad->tr.vertices.y = vertices[SP_VERTEX_Y3];
	quad->br.vertices.x = vertices[SP_VERTEX_X4];
	quad->br.vertices.y = vertices[SP_VERTEX_Y4];

	quad->bl.texCoords.u = self->uvs[SP_VERTEX_X1];
	quad->bl.texCoords.v = self->uvs[SP_VERTEX_Y1];
	quad->tl.texCoords.u = self->uvs[SP_VERTEX_X2];
	quad->tl.texCoords.v = self->uvs[SP_VERTEX_Y2];
	quad->tr.texCoords.u = self->uvs[SP_VERTEX_X3];
	quad->tr.texCoords.v = self->uvs[SP_VERTEX_Y3];
	quad->br.texCoords.u = self->uvs[SP_VERTEX_X4];
	quad->br.texCoords.v = self->uvs[SP_VERTEX_Y4];
}

}
