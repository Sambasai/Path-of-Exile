#include <bits/stdc++.h>
using namespace std;
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "game_level.h"
#include "text_renderer.h"

SpriteRenderer *Renderer;
GameObject *Player;
GameObject *Monster1;
GameObject *Monster2;
GameObject *Monster3;
GameObject *Monster4;
TextRenderer *Text;
int InitTime;
glm::vec2 player_pos;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
    // delete Renderer;
    delete Player;
}

void Game::ChangeLevel(int lvl)
{

    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("textures/bg1.jpg", false, "background");
    ResourceManager::LoadTexture("textures/gmovr.jpg", false, "gmovr");
    ResourceManager::LoadTexture("textures/gmwon.jpg", false, "gmwon");
    ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("textures/block.png", false, "block");
    ResourceManager::LoadTexture("textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("textures/sq.png", true, "paddle");
    int num = 25;
    if (lvl == 1)
    {
        num = 30;
    }
    else
    {
        num = 35;
    }

    vector<vector<int>> grid1(num, vector<int>(num, 0));
    // freopen("levels/two.lvl", "w", stdout);
    // float h = this->Levels[this->Level].height;
    // float w = this->Levels[this->Level].width;
    srand(time(0));
    int c = 0;
    for (int i = 0; i < num; i++)
    { //1225
        int sum = rand() % (num * num - num - 1);
        // cout << sum << endl;
        int x = sum / num;
        int y = sum % num;
        grid1[x][y] = rand() % 2 + 1;
        if (grid1[x][y] == 2)
        {
            // this->Levels[this->Level].Bricks[x * 25 + y].isCoin = true;
            c++;
        }
    }
    this->coins = c;
    // for (int i = 0; i < num; i++)
    // {
    //     for (int j = 0; j < num; j++)
    //     {
    //         cout << grid1[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // fclose(stdout);
    // load levels
    ofstream fout;
    fout.open("levels/two.lvl");
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
            fout << grid1[i][j] << " ";
        fout << "\n";
    }
    fout.close();
    GameLevel two;
    two.Load("levels/two.lvl", this->Width, this->Height);

    ////
    this->Levels.push_back(two);

    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    Player->Hor = 0;
    Player->Ver = 0;
    glm::vec2 monsterPos = glm::vec2(this->Width / 2.0f - MONSTER_SIZE.x / 2.0f, this->Height - 4 * MONSTER_SIZE.y);
    Monster1 = new GameObject(monsterPos, MONSTER_SIZE, ResourceManager::GetTexture("paddle"));
    Monster1->Velocity = glm::vec2(500.0f, 0.0f);
    this->Level = lvl;
    if (this->Level == 1)
    {
        glm::vec2 monsterPos = glm::vec2(this->Width / 2.0f - MONSTER_SIZE.x / 2.0f, this->Height - 8 * MONSTER_SIZE.y);
        Monster2 = new GameObject(monsterPos, MONSTER_SIZE, ResourceManager::GetTexture("paddle"));
        Monster2->Velocity = glm::vec2(500.0f, 0.0f);
    }
    else if (this->Level == 2)
    {
        glm::vec2 monsterPos = glm::vec2(this->Width / 2.0f - MONSTER_SIZE.x / 2.0f, this->Height - 14 * MONSTER_SIZE.y);
        Monster3 = new GameObject(monsterPos, MONSTER_SIZE, ResourceManager::GetTexture("paddle"));
        Monster3->Velocity = glm::vec2(500.0f, 0.0f);
        glm::vec2 monsterPos1 = glm::vec2(this->Width / 2.0f - MONSTER_SIZE.x / 2.0f, this->Height - 20 * MONSTER_SIZE.y);
        Monster4 = new GameObject(monsterPos1, MONSTER_SIZE, ResourceManager::GetTexture("paddle"));
        Monster4->Velocity = glm::vec2(500.0f, 0.0f);
    }
}

void Game::Init()
{
    this->light = 0;
    InitTime = time(0);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("fonts/OCRAEXT.TTF", 24);
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("textures/bg1.jpg", false, "background");
    ResourceManager::LoadTexture("textures/gmovr.jpg", false, "gmovr");
    ResourceManager::LoadTexture("textures/gmwon.jpg", false, "gmwon");
    ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("textures/block.png", false, "block");
    ResourceManager::LoadTexture("textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("textures/sq.png", true, "paddle");

    vector<vector<int>> grid(25, vector<int>(25, 0));
    freopen("levels/one.lvl", "w", stdout);
    // float h = this->Levels[this->Level].height;
    // float w = this->Levels[this->Level].width;
    srand(time(0));
    int c = 0;
    for (int i = 0; i < 25; i++)
    {
        int sum = rand() % 599;
        // cout << sum << endl;
        int x = sum / 25;
        int y = sum % 25;
        grid[x][y] = rand() % 2 + 1;
        if (grid[x][y] == 2)
        {
            // this->Levels[this->Level].Bricks[x * 25 + y].isCoin = true;
            c++;
        }
    }
    this->coins = c;
    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    fclose(stdout);
    // load levels
    GameLevel one;
    one.Load("levels/one.lvl", this->Width, this->Height);

    ////
    this->Levels.push_back(one);

    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    Player->Hor = 0;
    Player->Ver = 0;
    glm::vec2 monsterPos = glm::vec2(this->Width / 2.0f - MONSTER_SIZE.x / 2.0f, this->Height - 4 * MONSTER_SIZE.y);
    Monster1 = new GameObject(monsterPos, MONSTER_SIZE, ResourceManager::GetTexture("paddle"));
    Monster1->Velocity = glm::vec2(500.0f, 0.0f);
}

void Game::Update(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        this->player_pos = Player->Position;
    }
    // update objects
    // Player->Move(dt, this->Width);
    // check for collisions
    this->DoCollisions();
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // float mvelocity = MONSTER_VELOCITY * dt;

        if (Monster1->Position.x < 1.0f)
        {
            Monster1->Velocity = -Monster1->Velocity;
        }
        else if (Monster1->Position.x > this->Width - Monster1->Size.x - 1.0f)
        {
            Monster1->Velocity = -Monster1->Velocity;
        }
        float mvelocity = Monster1->Velocity.x * dt;
        Monster1->Position.x += mvelocity;

        if (this->Level == 1)
        {
            if (Monster2->Position.x < 1.0f)
            {
                Monster2->Velocity = -Monster2->Velocity;
            }
            else if (Monster2->Position.x > this->Width - Monster2->Size.x - 1.0f)
            {
                Monster2->Velocity = -Monster2->Velocity;
            }
            float mvelocity = Monster2->Velocity.x * dt;
            Monster2->Position.x += mvelocity;
        }
        else if (this->Level == 2)
        {
            if (Monster2->Position.x < 1.0f)
            {
                Monster2->Velocity = -Monster2->Velocity;
            }
            else if (Monster2->Position.x > this->Width - Monster2->Size.x - 1.0f)
            {
                Monster2->Velocity = -Monster2->Velocity;
            }
            float mvelocity = Monster2->Velocity.x * dt;
            Monster2->Position.x += mvelocity;
            if (Monster3->Position.x < 1.0f)
            {
                Monster3->Velocity = -Monster3->Velocity;
            }
            else if (Monster3->Position.x > this->Width - Monster3->Size.x - 1.0f)
            {
                Monster3->Velocity = -Monster3->Velocity;
            }
            float mvelocity2 = Monster3->Velocity.x * dt;
            Monster3->Position.x += mvelocity2;
            if (Monster4->Position.x < 1.0f)
            {
                Monster4->Velocity = -Monster4->Velocity;
            }
            else if (Monster4->Position.x > this->Width - Monster4->Size.x - 1.0f)
            {
                Monster4->Velocity = -Monster4->Velocity;
            }
            float mvelocity1 = Monster4->Velocity.x * dt;
            Monster4->Position.x += mvelocity1;
        }
        // cout << mvelocity << endl;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
            Player->Hor = 2;
            Player->Ver = 0;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
            Player->Hor = 1;
            Player->Ver = 0;
        }
        if (this->Keys[GLFW_KEY_W])
        {
            if (Player->Position.y >= 0.0f)
                Player->Position.y -= velocity;
            Player->Ver = 2;
            Player->Hor = 0;
        }
        if (this->Keys[GLFW_KEY_S])
        {
            Player->Ver = 1;
            Player->Hor = 0;
            if (this->coins == 0 && Player->Position.y >= this->Height - Player->Size.y)
            {
                // exit(0);
                if (this->Level == 2)
                {
                    this->State = GAME_WIN;
                }
                else
                {
                    this->ChangeLevel(this->Level + 1);
                }
            }
            if (Player->Position.y <= this->Height - Player->Size.y)
            {
                Player->Position.y += velocity;
            }
        }
    }
}

bool CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

// Direction VectorDirection(glm::vec2 target)
// {
//     glm::vec2 compass[] = {
//         glm::vec2(0.0f, 1.0f),  // up
//         glm::vec2(1.0f, 0.0f),  // right
//         glm::vec2(0.0f, -1.0f), // down
//         glm::vec2(-1.0f, 0.0f)  // left
//     };
//     float max = 0.0f;
//     unsigned int best_match = -1;
//     for (unsigned int i = 0; i < 4; i++)
//     {
//         float dot_product = glm::dot(glm::normalize(target), compass[i]);
//         if (dot_product > max)
//         {
//             max = dot_product;
//             best_match = i;
//         }
//     }
//     return (Direction)best_match;
// }

void Game::DoCollisions()
{
    if (CheckCollision(*Player, *Monster1))
    {
        // exit(0);
        this->State = GAME_OVER;
    }
    if (this->Level == 1)
    {
        if (CheckCollision(*Player, *Monster2))
        {
            // exit(0);
            this->State = GAME_OVER;
        }
    }
    else if (this->Level == 2)
    {
        if (CheckCollision(*Player, *Monster2))
        {
            // exit(0);
            this->State = GAME_OVER;
        }
        if (CheckCollision(*Player, *Monster3))
        {
            // exit(0);
            this->State = GAME_OVER;
        }
        if (CheckCollision(*Player, *Monster4))
        {
            // exit(0);
            this->State = GAME_OVER;
        }
    }
    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            if (CheckCollision(*Player, box))
            {
                if (!box.IsSolid)
                {
                    // cout << 1 << endl;
                    // exit(0);
                    if (box.isCoin)
                    {
                        box.isCoin = false;
                        this->coins = this->coins - 1;
                        // if (this->coins == 0)
                        // {
                        //     exit(0);
                        // }
                        if (this->light == 1)
                        {
                            this->score++;
                        }
                        this->score++;
                        // cout << this->coins << endl;
                    }
                    box.Destroyed = true;
                }
            }
        }
        if (box.IsSolid)
        {
            if (CheckCollision(*Player, box))
            {
                if (Player->Hor)
                {
                    if (Player->Position.x < box.Position.x && Player->Hor == 1)
                    {
                        Player->Position.x = box.Position.x - Player->Size.x - 1.0f;
                    }
                    else if (Player->Position.x >= box.Position.x && Player->Hor == 2)
                    {
                        Player->Position.x = box.Position.x + box.Size.x + 1.0f;
                    }
                }
                else if (Player->Ver)
                {
                    if (Player->Position.y < box.Position.y && Player->Ver == 1)
                    {
                        Player->Position.y = box.Position.y - Player->Size.y - 1.0f;
                    }
                    else if (Player->Position.y >= box.Position.y && Player->Ver == 2)
                    {
                        Player->Position.y = box.Position.y + box.Size.y + 1.0f;
                    }
                }
            }
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), player_pos, light);
        // draw level
        this->Levels[this->Level].Draw(*Renderer, player_pos, light);
        // draw player
        Player->Draw(*Renderer, player_pos, light);
        Monster1->Draw(*Renderer, player_pos, light);
        if (this->Level == 1)
        {
            Monster2->Draw(*Renderer, player_pos, light);
        }
        else if (this->Level == 2)
        {
            Monster2->Draw(*Renderer, player_pos, light);
            Monster3->Draw(*Renderer, player_pos, light);
            Monster4->Draw(*Renderer, player_pos, light);
        }
        std::stringstream ss, tt;
        ss << this->score;
        Text->RenderText("Score:" + ss.str(), 5.0f, 5.0f, 1.0f);
        tt << time(0) - InitTime;
        Text->RenderText("Time:" + tt.str(), 5.0f, 25.0f, 1.0f);
    }
    if (this->State == GAME_OVER)
    {
        Renderer->DrawSprite(ResourceManager::GetTexture("gmovr"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), player_pos, 0);
        // exit(0);
    }
    if (this->State == GAME_WIN)
    {
        Renderer->DrawSprite(ResourceManager::GetTexture("gmwon"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f, glm::vec3(1.0f), player_pos, 0);
        // exit(0);
    }
}