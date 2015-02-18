/*
 * Scene.h
 *
 *  Created on: 15 февр. 2015 г.
 *      Author: Admin
 */

#ifndef SOURCES_GRAPHICS_SCENE_H_
#define SOURCES_GRAPHICS_SCENE_H_


#include "Sprite.h"
#include "Viewport.h"

#include <vector>

namespace _2d {


template<typename TUnit, typename TCarrier>
class Scene : public Object<TUnit> {

	typedef typename std::vector<Sprite<TUnit, TCarrier>*/*, std::allocator<Sprite<TUnit>*>*/>::iterator sprite_iter;

	std::vector<Sprite<TUnit, TCarrier>*/*, std::allocator<Sprite<TUnit>*>*/> sprites;


public:

	void add(const Sprite<TUnit, TCarrier> &sprite) {
		sprites.insert(sprites.end(), &sprite);
	}

	void clear() {
		sprites.clear();
	}

	void render(Viewport<TUnit, TCarrier> &viewport) {
		rect_t<TUnit> rcViewport, rcSprite, rcDrawing;

		viewport.getBoundingRect(rcViewport);
		for (sprite_iter i = sprites.begin(), iend = sprites.end(); i != iend; ++i) {
			(*i)->getBoundingRect(rcSprite);

			offset(rcSprite, getX(), getY());
			if (intersect(rcViewport, rcSprite, rcDrawing)) {
				offset(rcDrawing, -rcViewport.getX(), -rcViewport.getY());

				(*i)->render(viewport, rcDrawing);
			}
		}
	}
};


}


#endif /* SOURCES_GRAPHICS_SCENE_H_ */
