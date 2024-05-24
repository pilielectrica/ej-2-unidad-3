#include "Game.h"
#include "Box2DHelper.h"
#include "b2ExSoftCircleBody.h"
#include "circulo.h"
#include "circulo.cpp"
#include <iostream>
using namespace std;
using namespace sf;
Game::Game(int ancho, int alto,std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
	wnd->setVisible(true);
	fps = 60;
	wnd->setFramerateLimit(fps);
	frameTime = 1.0f / fps;
	SetZoom();
	InitPhysics();
}

void Game::Loop()
{
	while (wnd->isOpen())
	{
		wnd->clear(clearColor);
		DoEvents();
		CheckCollitions();
		UpdatePhysics();
		DrawGame();
		wnd->display();
		
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8);
	phyWorld->ClearForces();
	phyWorld->DebugDraw();
}

void Game::DrawGame()
{ 
	

	_circulo = new Circulo(phyWorld, wnd);
	_circulo2 = new Circulo(phyWorld, wnd);

	_circulo->ActualizarPosiciones(controlBody); _circulo->Dibujar();

	
	_circulo2->ActualizarPosiciones(controlBody2); _circulo2->Dibujar();
	//_circulo->setScale();

	
}

void Game::DoEvents()
{
	Event evt;

	bool isDragging = false;
	sf::Vector2f offset;

	while (wnd->pollEvent(evt)) {
		switch (evt.type) {
		case sf::Event::Closed:
			wnd->close();
			break;
		case sf::Event::MouseButtonPressed:
			if (evt.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f mousePos = wnd->mapPixelToCoords(sf::Vector2i(evt.mouseButton.x, evt.mouseButton.y));
				if (_circulo->getglobalBoundsSprite().contains(mousePos)) {
					isDragging = true;
					offset = _circulo->getPosition() - mousePos;
				}
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (evt.mouseButton.button == sf::Mouse::Left) {
				isDragging = false;
			}
			break;
		case sf::Event::MouseMoved:
			if (isDragging) {
				sf::Vector2f mousePos = wnd->mapPixelToCoords(sf::Vector2i(evt.mouseMove.x, evt.mouseMove.y));
				sf::Vector2f newPos = mousePos + offset;
				_circulo->SetPosition(newPos, controlBody);
				 // Update the position of the dynamic body
			}
			break;
		}
	}



}

void Game::CheckCollitions()
{
	// Veremos mas adelante
}

// Definimos el area del mundo que veremos en nuestro juego
// Box2D tiene problemas para simular magnitudes muy grandes
void Game::SetZoom()
{
	View camara;
	// Posicion del view
	camara.setSize(100.0f, 100.0f);
	camara.setCenter(50.0f, 50.0f);
	wnd->setView(camara); // Asignamos la camara
}

void Game::InitPhysics()
{
	// Inicializamos el mundo con la gravedad por defecto
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	// Creamos el renderer de debug y le seteamos las banderas para que dibuje TODO
	debugRender = new SFMLRenderer(wnd);
	debugRender->SetFlags(UINT_MAX);
	phyWorld->SetDebugDraw(debugRender);

	// Creamos un piso y paredes
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	//creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);

	//------------------------------------
	//Creación del cuerpo blando
	//------------------------------------

	controlBody = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 10.0f, 3.0f, 1);
	controlBody->SetTransform(b2Vec2(20.0f, 50.0f), 0.0f);

	controlBody->SetAwake(true);

	controlBody2 = Box2DHelper::CreateCircularDynamicBody(phyWorld, 5, 10.0f, 3.0f, 1);
	controlBody2->SetTransform(b2Vec2(40.0f, 50.0f), 0.0f);

	controlBody2->SetAwake(true);

	b2DistanceJointDef jointDef;
	jointDef.Initialize(controlBody, controlBody2, controlBody->GetWorldCenter(), controlBody2->GetWorldCenter());
	jointDef.collideConnected = true;
	b2LinearStiffness(jointDef.stiffness, jointDef.damping, 2.0f, 0.5f, controlBody, controlBody2);

	phyWorld->CreateJoint(&jointDef);
	

}

Game::~Game(void)
{ }