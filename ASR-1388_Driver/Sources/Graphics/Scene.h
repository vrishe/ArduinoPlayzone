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

protected:

	typedef const Sprite<TUnit, TCarrier>			*sprite_ptr;
	typedef std::vector<sprite_ptr>					sprite_vector;
	typedef typename sprite_vector::const_iterator	sprite_const_iterator;
	typedef typename sprite_vector::iterator		sprite_iterator;

	sprite_vector sprites;


public:

	using typename Object<TUnit>::unit_t;
	using typename Object<TUnit>::uunit_t;


	Scene(unit_t x, unit_t y, uunit_t w, uunit_t h)
		: Object<TUnit>(x, y, w, h) {

		/* Nothing to do */
	}

	Scene(uunit_t w, uunit_t h)
		: Object<TUnit>(w, h) {

		/* Nothing to do */
	}

	Scene() : Object<TUnit>(0, 0, 0, 0) {
		/* Nothing to do */
	}

	virtual ~Scene() {
		/* Nothing to do */
	}


	void add(const Sprite<TUnit, TCarrier> &sprite) {
		sprites.insert(sprites.end(), &sprite);
	}

	virtual void clear() {
		sprites.clear();
	}

	void renderTo(Viewport<TUnit, TCarrier> &viewport) const {
		point_t<unit_t> origin;

		origin.x = this->Object<TUnit>::getX();
		origin.y = this->Object<TUnit>::getY();

		rect_t<unit_t> rcViewport, rcSprite, rcDrawing;

		viewport.getBoundingRect(rcViewport);
		for (sprite_const_iterator sprite_cur = sprites.begin(), 
			sprite_end = sprites.end(); sprite_cur != sprite_end; ++sprite_cur) {

			sprite_ptr sprite = *sprite_cur;
			sprite->getBoundingRect(rcSprite);

			offset(rcSprite, origin.x, origin.y);
			if (intersect(rcViewport, rcSprite, rcDrawing)) {
				sprite->render(viewport, rcDrawing, origin);
			}
		}
	}
};


}


#endif /* SOURCES_GRAPHICS_SCENE_H_ */
