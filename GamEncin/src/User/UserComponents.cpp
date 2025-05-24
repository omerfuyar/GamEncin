#include "GamEncin.h"

#pragma region Dialogue Objects

DialogueOption::DialogueOption(string text, DialoguePiece *nextPiece) : text(text), nextPiece(nextPiece) {}

string DialogueOption::GetText()
{
    return text;
}

DialoguePiece *const DialogueOption::GetNextDialogue()
{
    return nextPiece;
}

DialoguePiece::DialoguePiece(string text, DialogueOption *option1, DialogueOption *option2, DialogueOption *option3) : text(text)
{
    if (option1)
    {
        options.push_back(option1);
    }
    if (option2)
    {
        options.push_back(option2);
    }
    if (option3)
    {
        options.push_back(option3);
    }
}

string DialoguePiece::GetText()
{
    return text;
}

vector<DialogueOption *> DialoguePiece::GetOptions()
{
    return options;
}

Dialogue::Dialogue(string name, DialoguePiece *firstPiece, PlayerDialogueState nextState) : name(name), firstPiece(firstPiece), nextState(nextState) {}

DialoguePiece *const Dialogue::GetFirstPiece()
{
    return firstPiece;
}

PlayerDialogueState Dialogue::GetNextState()
{
    return nextState;
}

#pragma endregion

#pragma region DialoguePanelController

DialoguePanelController::DialoguePanelController(Object *obj) : Component(obj)
{
    Font *tamzenFont = FontManager::GetFont(Input::GetExeFilePath() + "/Resources/Fonts/Tamzen/Tamzen10x20b.bdf");
    Texture *texDialoguePanel = TextureManager::GetTexture(Input::GetExeFilePath() + "/Resources/Textures/UI/DialoguePanel.png");
    Texture *texAnswerPanel1 = TextureManager::GetTexture(Input::GetExeFilePath() + "/Resources/Textures/UI/AnswerPanel1.png");
    Texture *texAnswerPanel2 = TextureManager::GetTexture(Input::GetExeFilePath() + "/Resources/Textures/UI/AnswerPanel2.png");
    Texture *texAnswerPanel3 = TextureManager::GetTexture(Input::GetExeFilePath() + "/Resources/Textures/UI/AnswerPanel3.png");
    texDialoguePanel->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    texAnswerPanel1->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    texAnswerPanel2->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    texAnswerPanel3->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);

    Object &timerObj = object->CreateChildObject();
    timerObj.SetName("Timer");
    timerObj.GetTransform()->SetLocalPosition(Vector3(0, 3, 10));
    timerMesh = timerObj.AddComponent<TextMesh>();
    timerMesh->SetFont(tamzenFont);
    timerMesh->SetTextSize(Vector2::One() * 3.0f);

    Object &textPanelObj = object->CreateChildObject();
    textPanelObj.SetName("TextPanel");
    textPanelObj.GetTransform()->SetLocalPosition(Vector3(0, 1, -0.1f));
    textPanel = textPanelObj.AddComponent<ModelMesh>();
    textPanel->SetMeshData(MeshBuilder::CreatePlane(panelSize * 3.0f));
    textPanel->SetTexture(texDialoguePanel);

    Object &textObj = textPanelObj.CreateChildObject();
    textObj.SetName("Text");
    textObj.GetTransform()->SetLocalPosition(Vector3(0, 0, 0.1f));
    textMesh = textObj.AddComponent<TextMesh>();
    textMesh->SetFont(tamzenFont);
    textMesh->SetTextSize(Vector2::One() * 4.0f);

    Object &answerPanelObj = object->CreateChildObject();
    answerPanelObj.SetName("AnswerPanel");
    answerPanelObj.GetTransform()->SetLocalPosition(Vector3(0, -1.5f, -0.1f));
    answerPanel = answerPanelObj.AddComponent<ModelMesh>();
    answerPanel->SetMeshData(MeshBuilder::CreatePlane(panelSize * 2));
    answerPanel->SetTexture(TextureManager::GetTexture(Input::GetExeFilePath() + "/Resources/Textures/UI/AnswerPanel3.png"));

    float y_start_offset = 0.5f;
    float y_spacing = 0.5f;
    float z_offset = 0.1f;

    for (int i = 0; i < 3; ++i)
    {
        Object &optionObj = answerPanelObj.CreateChildObject();
        optionObj.SetName("Option" + to_string(i + 1));
        optionObj.GetTransform()->SetLocalPosition(Vector3(0, y_start_offset - (i * y_spacing), z_offset));
        optionPlaces[i] = optionObj.AddComponent<TextMesh>();
        optionPlaces[i]->SetFont(tamzenFont);
        optionPlaces[i]->SetTextSize(Vector2::One() * 3.0f);
    }
}

void DialoguePanelController::ActivatePanel()
{
    object->GetTransform()->SetLocalPosition(Vector3(-5, 0, 0));
}

void DialoguePanelController::DeactivatePanel()
{
    object->GetTransform()->SetLocalPosition(Vector3(5, 0, 0));
}

void DialoguePanelController::SetEnvironmentObj(Object *obj)
{
    environmentObj = obj;
}

void DialoguePanelController::AddEnemy(Enemy *obj)
{
    enemies.push_back(obj);
    obj->GetOwnerObject()->GetTransform()->SetLocalPosition(Vector3(0, 0, 10));
}

void DialoguePanelController::EnemyKilled(Enemy *enemy)
{
    killedEnemies++;

    if (killedEnemies >= enemiesToSpawn)
    {
        FinishEnemyChallenge();
    }
}

void DialoguePanelController::BringPiece(DialoguePiece *piece)
{
    if (!piece)
    {
        player->EndDialogue();
        return;
    }

    textMesh->SetText(piece->GetText());

    for (int i = 0; i < 3; ++i)
    {
        optionPlaces[i]->SetText("");
    }

    currentOptions.clear();

    const auto &options = piece->GetOptions();
    if (options.size() > 3)
    {
        Application::Stop(IndexOutOfRangeErr, "DialoguePiece cannot have more than 3 options.");
        return;
    }

    answerPanel->SetTexture(TextureManager::GetTexture(Input::GetExeFilePath() + "/Resources/Textures/UI/AnswerPanel" + to_string(options.size()) + ".png"));

    for (int i = 0; i < options.size() && i < 3; i++)
    {
        optionPlaces[i]->SetText(std::to_string(i + 1) + ": " + options[i]->GetText());
        currentOptions.push_back(options[i]);
    }
}

void DialoguePanelController::SelectOption(DialogueOption *option)
{
    string text = option->GetText();

    if (text == "Power (Strength)")
    {
        StartStrengthChallenge();
    }
    else if (text == "Grace (Dexterity)")
    {
        StartGraceChallenge();
    }
    else if (text == "..." || text == "What!!?!")
    {
        Application::Stop(Safe, "Application ended by user.");
        return;
    }

    BringPiece(option->GetNextDialogue());
}

void DialoguePanelController::StartStrengthChallenge()
{
    environmentObj->GetTransform()->SetLocalPosition(Vector3(0, 0, 10));
    player->SetDialogueState(PlayerDialogueState::AfterChoseStrength);

    for (Enemy *enemy : enemies)
    {
        enemy->GetOwnerObject()->GetTransform()->SetLocalPosition(Vector3(player->GetOwnerObject()->GetTransform()->GetGlobalPosition() + RandomVector2Direction() * enemySpawnRange + RandomVector2(), 0.0f));
        enemy->SetIsFollowing(true);
    }
}

void DialoguePanelController::FinishEnemyChallenge()
{
    // environmentObj->GetTransform()->SetLocalPosition(Vector3(0, 0, 0));
    player->SetDialogueState(PlayerDialogueState::AfterChallengeComplete);
    isInGraceChallenge = false;
    timerMesh->GetOwnerObject()->GetTransform()->SetLocalPosition(Vector3(0, 3, 10));
}

void DialoguePanelController::StartGraceChallenge()
{
    environmentObj->GetTransform()->SetLocalPosition(Vector3(0, 0, 10));
    player->SetDialogueState(PlayerDialogueState::AfterChoseGrace);
    isInGraceChallenge = true;
    timer = graceDuration;
    timerMesh->SetText(to_string(timer));
    timerMesh->GetOwnerObject()->GetTransform()->SetLocalPosition(Vector3(0, 3, -10));

    for (Enemy *enemy : enemies)
    {
        enemy->GetOwnerObject()->GetTransform()->SetLocalPosition(Vector3(player->GetOwnerObject()->GetTransform()->GetGlobalPosition() + RandomVector2Direction() * enemySpawnRange + RandomVector2(), 0.0f));
        enemy->SetIsFollowing(false);
        enemy->SetSpeed(enemy->GetSpeed() * enemy->GetGraceSpeedMultiplier());
    }
}

void DialoguePanelController::Update()
{

    if (Input::GetKey(Down, KeyCode::Num1) && currentOptions.size() > 0)
    {
        SelectOption(currentOptions[0]);
    }
    else if (Input::GetKey(Down, KeyCode::Num2) && currentOptions.size() > 1)
    {
        SelectOption(currentOptions[1]);
    }
    else if (Input::GetKey(Down, KeyCode::Num3) && currentOptions.size() > 2)
    {
        SelectOption(currentOptions[2]);
    }

    if (player->IsInDialogue())
    {
        return;
    }

    timerMesh->SetText(to_string(timer));
    if (isInGraceChallenge && timer <= 0.0f)
    {
        isInGraceChallenge = false;
        FinishEnemyChallenge();
        for (Enemy *enemy : enemies)
        {
            enemy->GetOwnerObject()->GetTransform()->SetLocalPosition(Vector3(0, 0, 10));
        }
        timer = graceDuration;
    }
    else
    {
        timer -= Application::GetDeltaTime();
    }
}

void DialoguePanelController::Start()
{
    player = object->GetScene()->FindFirstComponentByType<PlayerController>();
}

#pragma endregion

#pragma region Player

PlayerController::PlayerController(Object *obj) : Component(obj) {}

void PlayerController::StartDialogue(Dialogue *dialogue)
{
    if (dialogue == nullptr)
    {
        Application::Stop(NullPointerErr, "Dialogue is null.");
        return;
    }

    isInDialogue = true;
    playerDialoguePanel->ActivatePanel();
    playerDialoguePanel->BringPiece(dialogue->GetFirstPiece());

    if (dialogue->GetNextState() != PlayerDialogueState::Null)
    {
        SetDialogueState(dialogue->GetNextState());
    }
}

void PlayerController::EndDialogue()
{
    playerDialoguePanel->DeactivatePanel();
    isInDialogue = false;
}

void PlayerController::SetDialogueState(PlayerDialogueState state)
{
    currentDialogueState = state;
}

bool PlayerController::IsInDialogue()
{
    return isInDialogue;
}

void PlayerController::TakeDamage(int damage)
{
    health -= damage;

    if (health <= 0)
    {
        Application::Stop(Safe, "Application ended by user.");
        StartDialogue(new Dialogue("You died", new DialoguePiece("You died", new DialogueOption("...", nullptr)), PlayerDialogueState::End));
        return;
    }
}

void PlayerController::OnCollisionEnter(RigidBody *enteredRigidBody)
{
    AreaTrigger *areaTrigger = enteredRigidBody->GetOwnerObject()->GetComponent<AreaTrigger>();

    if (areaTrigger != nullptr && currentDialogueState == areaTrigger->GetStateToInteract())
    {
        StartDialogue(areaTrigger->GetDialogueToShow());
    }

    if (enteredRigidBody->GetOwnerObject()->GetTag() == "EnemyCollider")
    {
        TakeDamage(1);
        playerRB->AddForce((playerTR->GetGlobalPosition() - enteredRigidBody->GetOwnerObject()->GetTransform()->GetGlobalPosition()).Normalized() * enemyForce);
    }
}

void PlayerController::OnCollisionStay(RigidBody *stayingRigidBody)
{
    NPC *npc = stayingRigidBody->GetOwnerObject()->GetComponent<NPC>();
    Enemy *enemy = stayingRigidBody->GetOwnerObject()->GetComponent<Enemy>();

    if (currentNPC == nullptr && npc != nullptr)
    {
        currentNPC = npc;
    }

    if (currentEnemy == nullptr && enemy != nullptr && enemy->IsKillable())
    {
        currentEnemy = enemy;
    }
}

void PlayerController::OnCollisionExit(RigidBody *exitedRigidBody)
{
    if (currentNPC == exitedRigidBody->GetOwnerObject()->GetComponent<NPC>())
    {
        currentNPC = nullptr;
    }

    if (currentEnemy == exitedRigidBody->GetOwnerObject()->GetComponent<Enemy>())
    {
        currentEnemy = nullptr;
    }
}

void PlayerController::Awake()
{
    playerTR = object->GetTransform();
    playerRB = object->GetComponent<RigidBody>();
}

void PlayerController::Start()
{
    playerDialoguePanel = object->GetScene()->FindFirstComponentByType<DialoguePanelController>();
}

void PlayerController::Update()
{
    if (isInDialogue)
    {
        return;
    }

    Vector3 input = Input::GetMovementVector().Normalized();
    playerTR->AddPosition((Vector2)input * movementSpeed * Application::GetDeltaTime());

    if (currentNPC != nullptr && Input::GetKey(Down, KeyCode::E))
    {
        Dialogue *dialogue = currentNPC->GetDialogue(currentDialogueState);
        StartDialogue(dialogue);
    }

    if (currentEnemy != nullptr && Input::GetKey(Down, KeyCode::F))
    {
        currentEnemy->Die();
        currentEnemy = nullptr;
    }
}

#pragma endregion

#pragma region Other

NPC::NPC(Object *obj) : Component(obj) {}

void NPC::SetUnavailableDialogue(Dialogue *dialogue)
{
    unavailableDialogue = dialogue;
}

void NPC::AddDialogue(PlayerDialogueState stateToInteract, Dialogue *dialogue)
{
    dialogues[static_cast<int>(stateToInteract)] = dialogue;
}

Dialogue *NPC::GetDialogue(PlayerDialogueState state)
{
    auto it = dialogues.find(static_cast<int>(state));

    if (it != dialogues.end())
    {
        return it->second;
    }

    return unavailableDialogue;
}

Enemy::Enemy(Object *obj) : Component(obj) {}

float Enemy::GetSpeed()
{
    return speed;
}

float Enemy::GetGraceSpeedMultiplier()
{
    return graceSpeedMultiplier;
}

void Enemy::SetSpeed(float newSpeed)
{
    speed = newSpeed;
}

void Enemy::SetIsFollowing(bool isFollowing)
{
    this->isFollowing = isFollowing;
}

void Enemy::Die()
{
    object->GetScene()->FindFirstComponentByType<DialoguePanelController>()->EnemyKilled(this);
    object->GetTransform()->SetLocalPosition(Vector3(0, 0, 10));
}

bool Enemy::IsKillable()
{
    return isFollowing;
}

void Enemy::Start()
{
    player = object->GetScene()->FindFirstComponentByType<PlayerController>()->GetOwnerObject()->GetTransform();
}

void Enemy::Update()
{
    if (player->GetOwnerObject()->GetComponent<PlayerController>()->IsInDialogue())
    {
        return;
    }

    Vector2 pos = object->GetTransform()->GetGlobalPosition();
    Vector2 playerPos = player->GetGlobalPosition();
    if ((playerPos - pos).GetMagnitude() > 0.0f)
    {
        direction = (playerPos - pos).Normalized();
    }
    else
    {
        return;
    }

    if (isFollowing)
    {
        object->GetTransform()->AddPosition(direction * speed * Application::GetDeltaTime());
    }
    else if (canMove)
    {
        target = pos + (direction * moveRange);
        canMove = false;
        object->GetTransform()->AddPosition(direction * speed * Application::GetDeltaTime());
    }
    else if (pos == target)
    {
        canMove = true;
    }
    else
    {
        Vector2 step = MoveTowards(pos, target, speed * Application::GetDeltaTime()) - pos;
        object->GetTransform()->AddPosition(step);
    }
}

CameraController::CameraController(Object *obj) : Component(obj) {}

void CameraController::Start()
{
    cameraTR = object->GetTransform();
    camera = object->GetComponent<Camera>();
}

void CameraController::Update()
{
    camera->AddOrthographicSize(-Input::GetMouseScrollDelta());

    if (Input::GetKey(Press, KeyCode::Escape))
    {
        Application::Stop(Safe, "Application ended by user.");
        return;
    }

    if (Input::GetKey(Down, C))
    {
        Renderer::SetFullScreen(!Renderer::IsFullScreen());
    }

    if (Input::GetKey(Down, V))
    {
        Renderer::SetVSync(!Renderer::IsVSyncEnabled());
    }

    // if (Input::GetKey(Down, KeyCode::O))
    //{
    //     camera->SetPerspective(!camera->IsPerspective());
    // }
    //
    // if (!Input::GetMouseButton(Press, Left))
    //{
    //    glfwSetInputMode(Renderer::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //    return;
    //}
    //
    // glfwSetInputMode(Renderer::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //
    // Vector3 movement = Input::GetMovementVector();
    // Vector2 mouseDelta = Input::GetMousePositionDelta();
    //
    // float tempSpeed = Input::GetKey(Press, LeftShift) ? 20.0f : 10.0f;
    //
    // camera->AddCameraFOV(-Input::GetMouseScrollDelta());
    //// camera->AddOrthographicSize(-Input::GetMouseScrollDelta());
    //
    // Vector3 camPos = cameraTR->GetLocalPosition();
    // Vector3 camDir = cameraTR->GetDirection();
    //
    // cameraTR->AddPosition(camDir * tempSpeed * movement.y * Application::GetDeltaTime());
    // cameraTR->AddPosition(camDir.Cross(Vector3::Up()) * tempSpeed * movement.x * Application::GetDeltaTime());
    // cameraTR->AddPosition(Vector3::Up() * tempSpeed * movement.z * Application::GetDeltaTime());
    //
    // cameraTR->AddRotation(Vector3(-mouseDelta.y, mouseDelta.x, 0) * 7.5f * Application::GetDeltaTime());
    // cameraTR->SetLocalRotation(Vector3(Clamp(cameraTR->GetGlobalRotation().x, -89.0f, 89.0f), cameraTR->GetLocalRotation().y, cameraTR->GetLocalRotation().z));
}

TestComponent::TestComponent(Object *obj) : Component(obj) {}

AreaTrigger::AreaTrigger(Object *obj) : Component(obj) {}

void AreaTrigger::SetStateToInteract(PlayerDialogueState stateToInteract)
{
    this->stateToInteract = stateToInteract;
}

void AreaTrigger::SetDialogueToShow(Dialogue *dialogue)
{
    dialogueToShow = dialogue;
}

Dialogue *AreaTrigger::GetDialogueToShow()
{
    return dialogueToShow;
}

PlayerDialogueState AreaTrigger::GetStateToInteract()
{
    return stateToInteract;
}

#pragma endregion
