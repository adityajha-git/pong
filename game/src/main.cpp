#include "raylib.h"
#include <iostream>

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
	float x, y;
	int speed_x, speed_y, radius;

	void Draw() {
		DrawCircle(x, y, radius, WHITE);
	}

	void Update() {
		x += speed_x;
		y += speed_y;

		//checking collision
		if (y + radius >= GetScreenHeight() || y - radius <= 0) {
			speed_y *= -1;
		}
		if (x + radius >= GetScreenWidth()) {
			cpu_score++;
			ResetBall();
		}
		if (x - radius <= 0) {
			player_score++;
			ResetBall();
		}
	}

	void ResetBall() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int speed_choices[2] = { 1,-1 };
		speed_x *= speed_choices[GetRandomValue(0, 1)];
		speed_y *= speed_choices[GetRandomValue(0, 1)];
	}

};

class Paddle {
protected:
	void CollisionCheck() {
		if (y <= 0) {
			y = 0;
		}

		if (y + height >= GetScreenHeight()) {
			y = GetScreenHeight() - height;
		}

	}

public:
	float x, y;
	float width, height;
	int speed;

	void Draw() {
		DrawRectangle(x, y, width, height, WHITE);

	}

	void Update() {
		if (IsKeyDown(KEY_UP)) {
			y = y - speed;
		}

		if (IsKeyDown(KEY_DOWN)) {
			y = y + speed;
		}
		CollisionCheck();

	}
};

class CpuPaddle :public Paddle {
public:
	void Update(int ball_y) {
		if (y + height / 2 > ball_y) {
			y = y - speed;
		}

		if (y + height / 2 <= ball_y) {
			y = y + speed;
		}
		CollisionCheck();
	}
};

Ball ball;
Paddle player;
CpuPaddle cpu;


int main() {
	std::cout << "Game Starting........." << std::endl;
	int window_height = 800;
	int window_width = 1280;
	InitWindow(window_width, window_height, "Pong");
	SetTargetFPS(60);

	ball.radius = 20;
	ball.x = window_width / 2;
	ball.y = window_height / 2;
	ball.speed_x = 7;
	ball.speed_y = 7;

	player.x = 15;
	player.y = window_height / 2 - 60;
	player.width = 25;
	player.height = 120;
	player.speed = 6;

	cpu.x = window_width - 35;
	cpu.y = window_height / 2 - 60;
	cpu.width = 25;
	cpu.height = 120;
	cpu.speed = 6;


	while (WindowShouldClose() == false) {
		BeginDrawing();

		//updating
		ball.Update();
		player.Update();
		cpu.Update(ball.y);

		//checking collison betwwen ball and paddle
		if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,player.width,player.height })) {
			ball.speed_x *= -1;
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ cpu.x,cpu.y,cpu.width,cpu.height })) {
			ball.speed_x *= -1;
		}

		//Drawing
		ClearBackground(BLACK);
		ball.Draw();
		player.Draw();
		cpu.Draw();
		DrawLine(window_width / 2, 0, window_width / 2, window_height, WHITE);
		DrawText(TextFormat("%i", cpu_score), window_width / 4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", player_score), 3 * window_width / 4 - 20, 20, 80, WHITE);



		EndDrawing();

	}

	CloseWindow();
}