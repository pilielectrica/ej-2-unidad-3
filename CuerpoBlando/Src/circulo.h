#pragma once
#include "SFMLRenderer.h"
#include <tchar.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
class Circulo
{
private:
	// Sprite y su imagen la para representación gráfica
	Sprite* _sprite;
	Texture* _image;
	RenderWindow* wnd;
	b2World* world;
	// Body para representación física
	b2Body* _body;
	//...
public:
	Circulo(b2World* _world, RenderWindow* _wnd)
	{
		// Guardamos puntero a ventana para dibujar luego
		wnd = _wnd;
		_image = new Texture();
		_image->loadFromFile("Pelota.png");
		// Cargamos el sprite
		_sprite = new Sprite(*_image);
		_sprite->setScale(0.07f, 0.065f);

		// Definimos el body y lo creamo

		// Creamos su figura de colisión
		// en este caso suponemos que la figura de
		// colision es una caja cuadrada
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(20.0f, 20.0f);


		// Creamos el fixture, le seteamos
		// la figura de colision
		// y agregamos el fixture al body
	}

	// Metodo que posiciona el sprites
	// segun la posicion del body
	void ActualizarPosiciones(b2Body* _body)
	{
		b2Vec2 pos = _body->GetPosition();
		_sprite->setPosition(pos.x - 6, pos.y - 7);
	}

	// Metodo que dibuja el sprite
	void Dibujar()
	{
		wnd->draw(*_sprite);
	}

	// Otros metodos
	// relacionados al personaje
	void setScale()
	{
		_sprite->setScale(1, 1);
	}
	FloatRect getglobalBoundsSprite() const
	{
		return _sprite->getGlobalBounds();
	}
	Vector2f getPosition()
	{
		return	_sprite->getPosition();
	}
	void SetPosition(const sf::Vector2f& pos, b2Body* _body) {
		_sprite->setPosition(pos);
		_body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);
	}
};
