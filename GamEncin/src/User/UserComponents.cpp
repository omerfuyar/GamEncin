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
        options.push_back(option1);
    if (option2)
        options.push_back(option2);
    if (option3)
        options.push_back(option3);
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
    Font *tamzenFont = FontManager::GetFont("GamEncin/Resources/Fonts/Tamzen/Tamzen10x20b.bdf");
    Texture *texDialoguePanel = TextureManager::GetTexture("GamEncin/Resources/Textures/UI/DialoguePanel.png");
    Texture *texAnswerPanel1 = TextureManager::GetTexture("GamEncin/Resources/Textures/UI/AnswerPanel1.png");
    Texture *texAnswerPanel2 = TextureManager::GetTexture("GamEncin/Resources/Textures/UI/AnswerPanel2.png");
    Texture *texAnswerPanel3 = TextureManager::GetTexture("GamEncin/Resources/Textures/UI/AnswerPanel3.png");
    texDialoguePanel->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    texAnswerPanel1->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    texAnswerPanel2->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    texAnswerPanel3->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);

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
    answerPanel->SetTexture(TextureManager::GetTexture("GamEncin/Resources/Textures/UI/AnswerPanel3.png"));

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

void DialoguePanelController::BringPiece(DialoguePiece *piece)
{
    if (!piece)
    {
        object->GetScene()->FindFirstComponentByType<PlayerController>()->EndDialogue();
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

    answerPanel->SetTexture(TextureManager::GetTexture("GamEncin/Resources/Textures/UI/AnswerPanel" + to_string(options.size()) + ".png"));

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
        LaunchCombatChallenge();
    }
    else if (text == "Wisdom (Intelligence)")
    {
        StartPuzzleRiddle();
        return;
    }
    else if (text == "Grace (Dexterity)")
    {
        ActivateTrapZone();
        return;
    }
    else if (text == "..." || text == "What!!?!")
    {
        Application::Stop(Safe, "Application ended by user.");
        return;
    }

    BringPiece(option->GetNextDialogue());
}

void DialoguePanelController::LaunchCombatChallenge()
{
    // Implement combat challenge logic here
}

void DialoguePanelController::StartPuzzleRiddle()
{
    // Implement puzzle riddle logic here
}

void DialoguePanelController::ActivateTrapZone()
{
    // Implement trap zone logic here
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
        currentDialogueState = dialogue->GetNextState();
    }
}

void PlayerController::EndDialogue()
{
    playerDialoguePanel->DeactivatePanel();
    isInDialogue = false;
}

void PlayerController::OnCollisionEnter(RigidBody *enteredRigidBody)
{
    NPC *npc = enteredRigidBody->GetOwnerObject()->GetComponent<NPC>();
    AreaTrigger *areaTrigger = enteredRigidBody->GetOwnerObject()->GetComponent<AreaTrigger>();

    if (currentNPC == nullptr && npc != nullptr)
    {
        currentNPC = npc;
    }
    else if (areaTrigger != nullptr && currentDialogueState == areaTrigger->GetStateToInteract())
    {
        StartDialogue(areaTrigger->GetDialogueToShow());
    }
}

void PlayerController::OnCollisionExit(RigidBody *exitedRigidBody)
{
    if (currentNPC == exitedRigidBody->GetOwnerObject()->GetComponent<NPC>())
    {
        currentNPC = nullptr;
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

    Vector3 input = Input::GetMovementVector();
    playerTR->AddPosition((Vector2)input * movementSpeed * Application::GetDeltaTime());

    if (currentNPC != nullptr && Input::GetKey(Press, KeyCode::E))
    {
        Dialogue *dialogue = currentNPC->GetDialogue(currentDialogueState);
        StartDialogue(dialogue);
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

    if (Input::GetKey(Down, F))
    {
        Renderer::SetFullScreen(!Renderer::IsFullScreen());
    }

    if (Input::GetKey(Down, V))
    {
        Renderer::SetVSync(!Renderer::IsVSyncEnabled());
    }

    if (Input::GetKey(Down, KeyCode::O))
    {
        camera->SetPerspective(!camera->IsPerspective());
    }

    if (!Input::GetMouseButton(Press, Left))
    {
        glfwSetInputMode(Renderer::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }

    glfwSetInputMode(Renderer::GetMainWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Vector3 movement = Input::GetMovementVector();
    Vector2 mouseDelta = Input::GetMousePositionDelta();

    float tempSpeed = Input::GetKey(Press, LeftShift) ? 20.0f : 10.0f;

    camera->AddCameraFOV(-Input::GetMouseScrollDelta());
    // camera->AddOrthographicSize(-Input::GetMouseScrollDelta());

    Vector3 camPos = cameraTR->GetLocalPosition();
    Vector3 camDir = cameraTR->GetDirection();

    cameraTR->AddPosition(camDir * tempSpeed * movement.y * Application::GetDeltaTime());
    cameraTR->AddPosition(camDir.Cross(Vector3::Up()) * tempSpeed * movement.x * Application::GetDeltaTime());
    cameraTR->AddPosition(Vector3::Up() * tempSpeed * movement.z * Application::GetDeltaTime());

    cameraTR->AddRotation(Vector3(-mouseDelta.y, mouseDelta.x, 0) * 7.5f * Application::GetDeltaTime());
    cameraTR->SetLocalRotation(Vector3(Clamp(cameraTR->GetGlobalRotation().x, -89.0f, 89.0f), cameraTR->GetLocalRotation().y, cameraTR->GetLocalRotation().z));
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
