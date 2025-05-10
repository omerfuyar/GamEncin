#pragma once
#include "GamEncin.h"

struct DialogueOption;
struct DialoguePiece;
struct Dialogue;
class DialoguePanelController;
class PlayerController;
class NPC;
class Enemy;
class AreaTrigger;
class CameraController;
class TestComponent;

enum class PlayerDialogueState
{
    Start,
    AfterFirstAreaTrigger,
    AfterOracleAwakens,
    AfterChoseStrength,
    AfterChoseWisdom,
    AfterChoseGrace,
    AfterChallengeComplete,
    End,
    Null
};

#pragma region Dialogue Objects

struct DialogueOption
{
public:
    DialogueOption(string text, DialoguePiece *nextPiece);

    string GetText();
    DialoguePiece *const GetNextDialogue();

private:
    string text = "Default Option";
    DialoguePiece *nextPiece = nullptr;
};

struct DialoguePiece
{
public:
    DialoguePiece(string text, DialogueOption *option1, DialogueOption *option2 = nullptr, DialogueOption *option3 = nullptr);

    string GetText();
    vector<DialogueOption *> GetOptions();

private:
    string text = "Default Text";
    vector<DialogueOption *> options;
};

struct Dialogue
{
public:
    Dialogue(string name, DialoguePiece *firstPiece, PlayerDialogueState nextState);
    DialoguePiece *const GetFirstPiece();
    PlayerDialogueState GetNextState();

private:
    string name = "Default Dialogue";
    DialoguePiece *firstPiece = nullptr;
    PlayerDialogueState nextState = PlayerDialogueState::Null;
};

class DialoguePanelController : public Component
{
public:
    DialoguePanelController(Object *obj);

    void BringPiece(DialoguePiece *piece);
    void SelectOption(DialogueOption *option);

    void ActivatePanel();
    void DeactivatePanel();
    void SetEnvironmentObj(Object *obj);
    void AddEnemy(Enemy *obj);
    void EnemyKilled(Enemy *enemy);

private:
    int enemiesToSpawn = 3;
    int killedEnemies = 0;
    PlayerController *player = nullptr;
    Vector2 panelSize = Vector2(2.0f, 1.0f);
    TextMesh *textMesh = nullptr;
    ModelMesh *textPanel = nullptr;
    ModelMesh *answerPanel = nullptr;
    array<TextMesh *, 3> optionPlaces = {nullptr, nullptr, nullptr};
    vector<DialogueOption *> currentOptions;
    Object *environmentObj = nullptr;
    vector<Enemy *> enemies;

    void LaunchCombatChallenge();
    void ActivateTrapZone();
    void Update() override;
    void Start() override;
};

#pragma endregion

#pragma region Player

class PlayerController : public Component
{
public:
    PlayerController(Object *obj);

    void StartDialogue(Dialogue *dialogue);
    void EndDialogue();
    void SetDialogueState(PlayerDialogueState state);

private:
    int health = 3;
    bool isInDialogue = false;
    float movementSpeed = 2;
    PlayerDialogueState currentDialogueState = PlayerDialogueState::Start;
    DialoguePanelController *playerDialoguePanel = nullptr;
    Transform *playerTR = nullptr;
    RigidBody *playerRB = nullptr;
    NPC *currentNPC = nullptr;
    Enemy *currentEnemy = nullptr;

    void TakeDamage(int damage);

    void OnCollisionEnter(RigidBody *enteredRigidBody) override;
    void OnCollisionExit(RigidBody *exitedRigidBody) override;
    void Awake() override;
    void Start() override;
    void Update() override;
};

#pragma endregion

#pragma region Other

class NPC : public Component
{
public:
    NPC(Object *obj);

    void SetUnavailableDialogue(Dialogue *dialogue);
    void AddDialogue(PlayerDialogueState stateToInteract, Dialogue *dialogue);
    Dialogue *GetDialogue(PlayerDialogueState stateToInteract);

private:
    Dialogue *unavailableDialogue = nullptr;
    unordered_map<int, Dialogue *> dialogues;
};

class Enemy : public Component
{
public:
    Enemy(Object *obj);
    void SetIsFollowing(bool isFollowing);
    void Die();
    bool IsKillable();

private:
    bool isFollowing;
    bool canMove = true;
    float speed = 5.0f;
    Transform *player = nullptr;
    Vector2 target;
    Vector2 direction;

    void Start() override;
    void Update() override;
};

class CameraController : public Component
{
public:
    CameraController(Object *obj);

    void Start() override;
    void Update() override;

private:
    Transform *cameraTR = nullptr;
    Camera *camera = nullptr;
};

class TestComponent : public Component
{
public:
    TestComponent(Object *obj);
};

class AreaTrigger : public Component
{
public:
    AreaTrigger(Object *obj);

    void SetStateToInteract(PlayerDialogueState stateToInteract);
    void SetDialogueToShow(Dialogue *dialogue);
    Dialogue *GetDialogueToShow();
    PlayerDialogueState GetStateToInteract();

private:
    Dialogue *dialogueToShow = nullptr;
    PlayerDialogueState stateToInteract = PlayerDialogueState::Null;
};

#pragma endregion
