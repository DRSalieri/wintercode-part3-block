﻿/*					*\
		game.h
	游戏界面相关代码
	2020 wintercode
	by id:191220017
\*					*/

#pragma once
#ifndef GAME_H
#define GAME_H

#include "resources.h"

void BallUpdate(Rotatable* t);
void BoardUpdate(Sprite* t);
Sprite *background = new Sprite(0, 0, bgImg, DefaultShow, [](Sprite* t) {}, 640, 480);

Sprite *board = new Sprite(400, 410, boardImg, DefaultShow, BoardUpdate, 640 / 6.0, 433 / 6.0);
Sprite *test = new Sprite(0, 0, testImg, DefaultShow, [](Sprite* t) {t->x = x; t->y = y; }, 30, 30);

Sprite *borderLeft = new Sprite(30 - 100, 15, boardImg, INVISIBLE_SHOW, DefaultUpdate, 100, 450);
Sprite *borderRight = new Sprite(415, 15, boardImg, INVISIBLE_SHOW, DefaultUpdate, 100, 450);
Sprite *borderUp = new Sprite(30, 15 - 100, boardImg, INVISIBLE_SHOW, DefaultUpdate, 385, 100);
Sprite *borderDown = new Sprite(30, 465, boardImg, INVISIBLE_SHOW, DefaultUpdate, 385, 2);

//Block *block1 = new Block(100, 50, BlockShow, BlockUpdate, 4, 100,100);
std::list<Block*> blocks;

double ballSpeed = 5;
double tSpeed = -0.4;
Vec2 ballVelocity(tSpeed, sqrt(1 - tSpeed * tSpeed));//which is always an unit vector
void BallUpdate(Rotatable* t) {
	//update the image for showing
	if ((gameTimer) / 10 % 2 == 0)
		t->image = &ballBlueImg;
	else t->image = &ballRedImg;

	//hitting the border
	if (isHit(t, borderLeft) || isHit(t, borderRight))ballVelocity.x = -ballVelocity.x;
	if (isHit(t, borderDown) || isHit(t, borderUp))ballVelocity.y = -ballVelocity.y;
	while (isHit(t, borderLeft))t->x += 1;
	while (isHit(t, borderRight))t->x -= 1;

	//hitting the block
	for (auto i = blocks.begin(); i != blocks.end(); i++)
		if (isHitCircle(t, (*i))) {
			Vec2 normal(Vec2(t->x + 0.5*t->width, t->y + 0.5*t->width)
				- Vec2((*i)->x + 0.5*(*i)->width, (*i)->y + 0.5*(*i)->width));
			Unitize(normal);
			ballVelocity = ballVelocity - 2 * (ballVelocity * normal) * normal;
			while (isHitCircle(t, (*i))) {
				t->x += normal.x;
				t->y += normal.y;
			}
			(*i)->rank--;
			break;
		}

	//hitting the board
	if (isHitCircle(t, board)) {
		Vec2 normal(Vec2(t->x + 0.5*t->width, t->y + 0.5*t->width)
			- Vec2(board->x + 0.5*board->width, board->y + 0.5*board->width));
		Unitize(normal);
		ballVelocity = ballVelocity - 2 * (ballVelocity * normal) * normal;
		while (isHitCircle(t, board)) {
			t->x += normal.x;
			t->y += normal.y;
		}
		//ballVelocity.y = -ballVelocity.y;

	}

	t->x += ballSpeed * ballVelocity.x;
	t->y += ballSpeed * ballVelocity.y;
	/*
	t->brush->SetTransform(
	D2D1::Matrix3x2F::Translation(x, y)*
	D2D1::Matrix3x2F::Rotation(t->angle,
	D2D1::Point2F((t->rec1.right + t->rec1.left) / 2 + x,
	(t->rec1.top + t->rec1.bottom) / 2 + y)));
	HRESULT	hr = myGFactory->GetFactory()->CreateTransformedGeometry(
	t->Grec,
	D2D1::Matrix3x2F::Translation(x, y)*
	D2D1::Matrix3x2F::Rotation(t->angle,
	D2D1::Point2F((t->rec1.right + t->rec1.left) / 2 + x,
	(t->rec1.top + t->rec1.bottom) / 2 + y)),
	//D2D1::Point2F(0, 0)),
	&t->Grec2
	);
	*/
}

void BallShow(Rotatable* t) {
	myGFactory->DrawBitmap(*(t->image), t->x, t->y, t->x + t->width, t->y + t->height);
	//pc0myGFactory->GetHandle()->FillGeometry(t->Grec, t->brush);
}

Rotatable *ball = new Rotatable(100, 400, ballRedImg, BallShow, BallUpdate, 20, 20);

void BoardUpdate(Sprite* t) {
	hitLeft = isHit(t, borderLeft);
	hitRight = isHit(t, borderRight);
	t->x = x * 2;
}

Sprite* mask = new Sprite(0, 0, maskImg, DefaultShow, DefaultUpdate, 600, 600);
Sprite* index = new Sprite(430, 30, indexImg, DefaultShow, DefaultUpdate, 131 * 0.5, 320 * 0.5);
GameString* hiScoreStr = new GameString(500, 30, 0.48);
GameString* gameScoreStr = new GameString(500, 55, 0.48);


void LoadObject() {


	//pool.AddSon(test);
	gamePool.AddSon(mask);
	gamePool.AddSon(board);

	gamePool.AddSon(borderLeft);
	gamePool.AddSon(borderRight);
	gamePool.AddSon(borderUp);
	gamePool.AddSon(borderDown);

	//pool.AddSon(block1);

	std::ifstream in(L"./data/1.blk");
	int num;
	in >> num;
	for (int i = 1; i <= num; i++) {
		float tx, ty;
		int rank;
		in >> tx >> ty >> rank;
		blocks.push_back(new Block(tx, ty, BlockShow, BlockUpdate, rank));
		gamePool.AddSon(blocks.back());
	}


	HRESULT t1 = myGFactory->GetHandle()->CreateBitmapBrush(ball->image->GetBitmap(), &ball->brush);
	HRESULT t2 = myGFactory->GetFactory()->CreateRectangleGeometry(ball->rec1, &(ball->Grec));
	//if (SUCCEEDED(t1) && SUCCEEDED(t2))
	gamePool.AddSon(background);
	gamePool.AddSon(index);
	gamePool.AddSon(hiScoreStr);
	gamePool.AddSon(gameScoreStr);
	hiScoreStr->str = "000000000";
	gameScoreStr->str = "000000000";
	gamePool.AddSon(ball);
}

#endif // !GAME_H
