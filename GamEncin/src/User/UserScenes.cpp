#include "GamEncin.h"

#pragma region UserScene1

TestScene1::TestScene1(string name) : Scene(name) {}

void TestScene1::BuildScene()
{
    //////////////// resources

    Font *myFont1 = FontManager::GetFont("GamEncin/Resources/Fonts/Garamond/garamond.bdf");
    Font *myFont2 = FontManager::GetFont("GamEncin/Resources/Fonts/Tamzen/Tamzen10x20b.bdf");
    Texture *texSamed = TextureManager::GetTexture("GamEncin/Resources/Textures/test.jpg");
    Texture *texCheckBoard = TextureManager::GetTexture("GamEncin/Resources/Textures/test3.jpg");

    /////////////// camera

    Object &cameraObj = CreateAndUseCameraObject();
    Camera *camera = cameraObj.GetComponent<Camera>();
    camera->SetCameraFOV(50.0f);
    camera->SetPerspective(false);
    camera->SetOrthographicSize(5.0f);
    camera->SetClipPlanes(0.1f, 300.0f);

    Transform *cameraTR = cameraObj.GetTransform();
    cameraTR->AddRotation(Vector3(0, -90, 0));
    cameraTR->AddPosition(Vector3(0, 0, 5));

    CameraController *camController = cameraObj.AddComponent<CameraController>();

    ///////////////// static Object

    Object &sphere1 = CreateObject();
    sphere1.SetName("Static Sphere");
    ModelMesh *sphereMesh1 = sphere1.AddComponent<ModelMesh>();
    sphereMesh1->SetMeshData(MeshBuilder::CreatePlane());
    sphereMesh1->SetTexture(texSamed);

    RigidBody *sphereRB1 = sphere1.AddComponent<RigidBody>();
    sphereRB1->SetDynamic(false);
    sphereRB1->SetGravityScale(0);

    ///////////////// player Object

    Object &playerObj = CreateObject();
    playerObj.SetName("Player Sphere");
    ModelMesh *sphereMesh2 = playerObj.AddComponent<ModelMesh>();
    sphereMesh2->SetMeshData(MeshBuilder::CreateSphere(1.0f));
    sphereMesh2->SetTexture(texCheckBoard);

    RigidBody *playerRB = playerObj.AddComponent<RigidBody>();
    playerRB->SetDynamic(true);
    playerRB->SetGravityScale(0);

    Transform *playerTR = playerObj.GetTransform();
    playerTR->AddPosition(Vector3(2.5f, 0, 0));

    PlayerController *playerController = playerObj.AddComponent<PlayerController>();

    ///////////////// dynamic test sphere

    Object &sphere3 = CreateObject();
    sphere3.SetName("Dynamic Sphere");
    ModelMesh *sphereMesh3 = sphere3.AddComponent<ModelMesh>();
    sphereMesh3->SetMeshData(MeshBuilder::CreateSphere(1.0f));
    sphereMesh3->SetTexture(texSamed);

    RigidBody *sphereRB3 = sphere3.AddComponent<RigidBody>();
    sphereRB3->SetDynamic(true);
    sphereRB3->SetGravityScale(1.0f);

    Transform *sphereTR3 = sphere3.GetTransform();
    sphereTR3->AddPosition(Vector3(0, 2.5f, 0));

    ///////////////// text

    Object &textObj1 = playerObj.CreateChildObject();
    textObj1.SetName("Player Text");
    textObj1.GetTransform()->SetLocalPosition(Vector3(0, 0, 2));
    TextMesh *text1 = textObj1.AddComponent<TextMesh>();
    text1->SetFont(myFont1);
    text1->SetCharDistance(1.5f);
    text1->SetTextSize(Vector2(5, 5));
    text1->SetText("Player");

    Object &textObj2 = sphere1.CreateChildObject();
    textObj2.SetName("Static Text");
    textObj2.GetTransform()->SetLocalPosition(Vector3(0, 0, 2));
    TextMesh *text2 = textObj2.AddComponent<TextMesh>();
    text2->SetFont(myFont2);
    text2->SetCharDistance(1.5f);
    text2->SetTextSize(Vector2(5, 5));
    text2->SetText("Ne Diyo\nLaa");

    Object &textObj3 = sphere3.CreateChildObject();
    textObj3.SetName("Dynamic Text");
    textObj3.GetTransform()->SetLocalPosition(Vector3(0, 0, 2));
    TextMesh *text3 = textObj3.AddComponent<TextMesh>();
    text3->SetFont(myFont2);
    text3->SetCharDistance(1.5f);
    text3->SetTextSize(Vector2(5, 5));
    text3->SetText("OOO 0000 OOOluum");
}

#pragma endregion

#pragma region UserScene2

TestScene2::TestScene2(string name) : Scene(name) {}

void TestScene2::BuildScene()
{
    //////////////// resources

    Font *myFont1 = FontManager::GetFont("GamEncin/Resources/Fonts/Garamond/garamond.bdf");
    Font *myFont2 = FontManager::GetFont("GamEncin/Resources/Fonts/Tamzen/Tamzen10x20b.bdf");
    Texture *texSamed = TextureManager::GetTexture("GamEncin/Resources/Textures/test.jpg");
    Texture *texCheckBoard = TextureManager::GetTexture("GamEncin/Resources/Textures/test3.jpg");
    Texture *texPlayer = TextureManager::GetTexture("GamEncin/Resources/Textures/Player.png");
    Texture *texOracle = TextureManager::GetTexture("GamEncin/Resources/Textures/Oracle.png");
    Texture *texWall = TextureManager::GetTexture("GamEncin/Resources/Textures/Wall.png");

    texPlayer->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    texOracle->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
    texWall->SetWrapAndFilter(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);

    //////////////// camera

    Object &cameraObj = CreateAndUseCameraObject();
    Camera *camera = cameraObj.GetComponent<Camera>();
    camera->SetCameraFOV(50.0f);
    camera->SetPerspective(false);
    camera->SetOrthographicSize(5.0f);
    camera->SetClipPlanes(0.1f, 300.0f);

    Transform *cameraTR = cameraObj.GetTransform();
    cameraTR->AddRotation(Vector3(0, -90, 0));
    cameraTR->AddPosition(Vector3(0, 0, 10));

    CameraController *camController = cameraObj.AddComponent<CameraController>();

    ////////////////// Environment

    Object &envObj = CreateObject();
    envObj.SetName("Environment");

#pragma region Walls

    for (int i = 0; i < 4; i++)
    {
        Object &wallObj = envObj.CreateChildObject();
        wallObj.SetName("Wall");
        wallObj.GetTransform()->SetLocalPosition(Vector3(-2 + i, -2, 0));
        ModelMesh *wallMesh = wallObj.AddComponent<ModelMesh>();
        wallMesh->SetMeshData(MeshBuilder::CreatePlane(Vector3::One()));
        wallMesh->SetTexture(texWall);
        RigidBody *wallRB = wallObj.AddComponent<RigidBody>();
        wallRB->SetDynamic(false);
        wallRB->SetColliderRadius(0.5f);
        wallRB->SetTrigger(false);
    }

    for (int i = 0; i < 10; i++)
    {
        Object &wallObj = envObj.CreateChildObject();
        wallObj.SetName("Wall");
        wallObj.GetTransform()->SetLocalPosition(Vector3(-2, -2 + i, 0));
        ModelMesh *wallMesh = wallObj.AddComponent<ModelMesh>();
        wallMesh->SetMeshData(MeshBuilder::CreatePlane(Vector3::One()));
        wallMesh->SetTexture(texWall);
        RigidBody *wallRB = wallObj.AddComponent<RigidBody>();
        wallRB->SetDynamic(false);
        wallRB->SetColliderRadius(0.5f);
        wallRB->SetTrigger(false);

        Object &wallObj1 = envObj.CreateChildObject();
        wallObj1.SetName("Wall");
        wallObj1.GetTransform()->SetLocalPosition(Vector3(2, -2 + i, 0));
        ModelMesh *wallMesh1 = wallObj1.AddComponent<ModelMesh>();
        wallMesh1->SetMeshData(MeshBuilder::CreatePlane(Vector3::One()));
        wallMesh1->SetTexture(texWall);
        RigidBody *wallRB1 = wallObj1.AddComponent<RigidBody>();
        wallRB1->SetDynamic(false);
        wallRB1->SetColliderRadius(0.5f);
        wallRB1->SetTrigger(false);
    }

    for (int i = 0; i < 4; i++)
    {
        Object &wallObj = envObj.CreateChildObject();
        wallObj.SetName("Wall");
        wallObj.GetTransform()->SetLocalPosition(Vector3(2 + i, 8, 0));
        ModelMesh *wallMesh = wallObj.AddComponent<ModelMesh>();
        wallMesh->SetMeshData(MeshBuilder::CreatePlane(Vector3::One()));
        wallMesh->SetTexture(texWall);
        RigidBody *wallRB = wallObj.AddComponent<RigidBody>();
        wallRB->SetDynamic(false);
        wallRB->SetColliderRadius(0.5f);
        wallRB->SetTrigger(false);

        Object &wallObj1 = envObj.CreateChildObject();
        wallObj1.SetName("Wall");
        wallObj1.GetTransform()->SetLocalPosition(Vector3(-2 - i, 8, 0));
        ModelMesh *wallMesh1 = wallObj1.AddComponent<ModelMesh>();
        wallMesh1->SetMeshData(MeshBuilder::CreatePlane(Vector3::One()));
        wallMesh1->SetTexture(texWall);
        RigidBody *wallRB1 = wallObj1.AddComponent<RigidBody>();
        wallRB1->SetDynamic(false);
        wallRB1->SetColliderRadius(0.5f);
        wallRB1->SetTrigger(false);
    }

    for (int i = 0; i < 6; i++)
    {
        Object &wallObj = envObj.CreateChildObject();
        wallObj.SetName("Wall");
        wallObj.GetTransform()->SetLocalPosition(Vector3(5, 8 + i, 0));
        ModelMesh *wallMesh = wallObj.AddComponent<ModelMesh>();
        wallMesh->SetMeshData(MeshBuilder::CreatePlane(Vector3::One()));
        wallMesh->SetTexture(texWall);
        RigidBody *wallRB = wallObj.AddComponent<RigidBody>();
        wallRB->SetDynamic(false);
        wallRB->SetColliderRadius(0.5f);
        wallRB->SetTrigger(false);

        Object &wallObj1 = envObj.CreateChildObject();
        wallObj1.SetName("Wall");
        wallObj1.GetTransform()->SetLocalPosition(Vector3(-5, 8 + i, 0));
        ModelMesh *wallMesh1 = wallObj1.AddComponent<ModelMesh>();
        wallMesh1->SetMeshData(MeshBuilder::CreatePlane(Vector3::One()));
        wallMesh1->SetTexture(texWall);
        RigidBody *wallRB1 = wallObj1.AddComponent<RigidBody>();
        wallRB1->SetDynamic(false);
        wallRB1->SetColliderRadius(0.5f);
        wallRB1->SetTrigger(false);
    }

    for (int i = 0; i < 6; i++)
    {
        Object &wallObj = envObj.CreateChildObject();
        wallObj.SetName("Wall");
        wallObj.GetTransform()->SetLocalPosition(Vector3(5 - i, 14, 0));
        ModelMesh *wallMesh = wallObj.AddComponent<ModelMesh>();
        wallMesh->SetMeshData(MeshBuilder::CreatePlane(Vector3::One()));
        wallMesh->SetTexture(texWall);
        RigidBody *wallRB = wallObj.AddComponent<RigidBody>();
        wallRB->SetDynamic(false);
        wallRB->SetColliderRadius(0.5f);
        wallRB->SetTrigger(false);

        Object &wallObj1 = envObj.CreateChildObject();
        wallObj1.SetName("Wall");
        wallObj1.GetTransform()->SetLocalPosition(Vector3(-5 + i, 14, 0));
        ModelMesh *wallMesh1 = wallObj1.AddComponent<ModelMesh>();
        wallMesh1->SetMeshData(MeshBuilder::CreatePlane(Vector3::One()));
        wallMesh1->SetTexture(texWall);
        RigidBody *wallRB1 = wallObj1.AddComponent<RigidBody>();
        wallRB1->SetDynamic(false);
        wallRB1->SetColliderRadius(0.5f);
        wallRB1->SetTrigger(false);
    }

#pragma endregion

    //////////////// UI object

    Object &uiObj = cameraObj.CreateChildObject();
    uiObj.SetName("UI Object");

    Transform *uiTR = uiObj.GetTransform();
    uiTR->SetLocalRotation(Vector3(0, 90, 0));

    DialoguePanelController *dialoguePanelController = uiObj.AddComponent<DialoguePanelController>();
    dialoguePanelController->DeactivatePanel();
    dialoguePanelController->SetEnvironmentObj(&envObj);

#pragma region Enemies

    for (int i = 0; i < 3; ++i)
    {
        Object &enemyObj = CreateObject();
        enemyObj.GetTransform()->SetLocalPosition(Vector3(0, 0, 10));

        Object &enemyColliderObj = enemyObj.CreateChildObject();
        enemyColliderObj.SetTag("EnemyCollider");
        RigidBody *enemyColliderRB = enemyColliderObj.AddComponent<RigidBody>();
        enemyColliderRB->SetDynamic(false);
        enemyColliderRB->SetColliderRadius(0.5f);

        ModelMesh *enemyMesh = enemyObj.AddComponent<ModelMesh>();
        enemyMesh->SetMeshData(MeshBuilder::CreatePlane(Vector2::One()));
        enemyMesh->SetTexture(TextureManager::GetTexture("GamEncin/Resources/Textures/Enemy.png"));

        RigidBody *enemyRB = enemyObj.AddComponent<RigidBody>();
        enemyRB->SetTrigger(true);
        enemyRB->SetColliderRadius(2.0f);
        enemyRB->SetDynamic(false);

        Enemy *enemy = enemyObj.AddComponent<Enemy>();

        dialoguePanelController->AddEnemy(enemy);
    }

#pragma endregion

    ///////////////// Player

    Object &playerObj = CreateObject();
    playerObj.SetName("Player");

    ModelMesh *sphereMesh22 = playerObj.AddComponent<ModelMesh>();
    sphereMesh22->SetMeshData(MeshBuilder::CreatePlane(Vector2::One()));
    sphereMesh22->SetTexture(texPlayer);

    RigidBody *playerRB2 = playerObj.AddComponent<RigidBody>();
    playerRB2->SetDynamic(true);
    playerRB2->SetGravityScale(0);
    playerRB2->SetColliderRadius(0.5f);

    Transform *playerTR2 = playerObj.GetTransform();
    cameraTR->SetParent(playerTR2);

    PlayerController *playerController2 = playerObj.AddComponent<PlayerController>();

    ///////////////// interactbles

    Object &areaTriggerObj = CreateObject();
    areaTriggerObj.SetName("Area Trigger");
    areaTriggerObj.SetTag("AreaTrigger");
    areaTriggerObj.GetTransform()->SetLocalPosition(Vector3(0, 12, 0));
    RigidBody *areaTriggerRB = areaTriggerObj.AddComponent<RigidBody>();
    areaTriggerRB->SetDynamic(false);
    areaTriggerRB->SetTrigger(true);
    areaTriggerRB->SetColliderRadius(5.0f);

    Object &oracleObj = CreateObject();
    oracleObj.SetName("Oracle");

    Transform *oracleTR = oracleObj.GetTransform();
    oracleTR->AddPosition(Vector3(3, 12, 0));

    ModelMesh *oracleMesh = oracleObj.AddComponent<ModelMesh>();
    oracleMesh->SetMeshData(MeshBuilder::CreatePlane(Vector2::One()));
    oracleMesh->SetTexture(texOracle);

    RigidBody *oracleRB = oracleObj.AddComponent<RigidBody>();
    oracleRB->SetDynamic(false);
    oracleRB->SetGravityScale(0);
    oracleRB->SetTrigger(true);
    oracleRB->SetColliderRadius(1.0f);

    Object &oracleColliderObj = oracleObj.CreateChildObject();
    oracleColliderObj.SetName("Oracle Collider");
    RigidBody *oracleColliderRB = oracleColliderObj.AddComponent<RigidBody>();
    oracleColliderRB->SetDynamic(false);
    oracleColliderRB->SetGravityScale(0);
    oracleColliderRB->SetTrigger(false);
    oracleColliderRB->SetColliderRadius(0.5f);

    NPC *oracle = oracleObj.AddComponent<NPC>();
    //------------ Base Dialogue Options
    DialogueOption *leaveDO = new DialogueOption("Leave...", nullptr);
    DialogueOption *continueDO = new DialogueOption("Continue...", nullptr);
    DialogueOption *iSeeDO = new DialogueOption("I see...", nullptr);
    DialogueOption *quitDO = new DialogueOption("...", nullptr);

    //------------ Oracle Final Speak
    DialogueOption *whatDO = new DialogueOption("What!!?!", nullptr);

    DialoguePiece *ending1DP = new DialoguePiece("You shall fall not by blade,\nbut by betrayal.\nIt begins now...", whatDO);
    DialoguePiece *ending2DP = new DialoguePiece("Then carve your name in the world.\nBut know: to be remembered,\nyou must be feared.", quitDO);
    DialoguePiece *ending3DP = new DialoguePiece("You may walk freely,\nbut even the wind carries\nchains unseen...", quitDO);

    DialogueOption *question1DO = new DialogueOption("How will I die?", ending1DP);
    DialogueOption *question2DO = new DialogueOption("What must I do to be remembered?", ending2DP);
    DialogueOption *question3DO = new DialogueOption("Can I be free of fate?", ending3DP);

    DialoguePiece *endQuestionDP = new DialoguePiece("You may ask one question.\nSpeak, and I shall answer — truthfully,\nbut only once.", question1DO, question2DO, question3DO);
    Dialogue *oracleFinalD = new Dialogue("Oracle Final", endQuestionDP, PlayerDialogueState::End);
    oracle->AddDialogue(PlayerDialogueState::AfterChallengeComplete, oracleFinalD);

    //------------ Each path should now trigger a scene change or challenge
    DialogueOption *iKnow = new DialogueOption("I know...", endQuestionDP);
    DialoguePiece *correctDP = new DialoguePiece("Correct. Time spares no flame.", iKnow);
    DialoguePiece *wrongDP = new DialoguePiece("Wrong. Looks like you are\nnot ready to proceed.", quitDO);

    DialogueOption *moonDO = new DialogueOption("1. Moon", wrongDP);
    DialogueOption *oceanDO = new DialogueOption("2. Ocean", wrongDP);
    DialogueOption *timeDO = new DialogueOption("3. Time", correctDP);

    DialoguePiece *wisdomRiddleDP = new DialoguePiece("Sun devours night.\nNight devours flame.\nFlame devours air.\nWhat devours the sun?", moonDO, oceanDO, timeDO);

    //------------ Oracle Trial Choice
    DialoguePiece *strengthPathDP = new DialoguePiece("You choose the way of the blade.\nSteel shall be your key.", iSeeDO);
    DialogueOption *imReady = new DialogueOption("I am ready...", wisdomRiddleDP);
    DialoguePiece *wisdomPathDP = new DialoguePiece("You choose the path of insight.\nSolve what others fear to think.", imReady);
    DialoguePiece *gracePathDP = new DialoguePiece("You choose the art of motion.\nSurvive the dance of danger.", iSeeDO);

    DialogueOption *chooseStrengthDO = new DialogueOption("Power (Strength)", strengthPathDP);
    DialogueOption *chooseWisdomDO = new DialogueOption("Wisdom (Intelligence)", wisdomPathDP);
    DialogueOption *chooseGraceDO = new DialogueOption("Grace (Dexterity)", gracePathDP);

    DialoguePiece *trialPromptDP = new DialoguePiece("To speak to me is to prove your worth.\nThree paths lie before you:\nPower. Wisdom. Grace.\nChoose...", chooseStrengthDO, chooseWisdomDO, chooseGraceDO);

    Dialogue *oracleTrialD = new Dialogue("Oracle Trial", trialPromptDP, PlayerDialogueState::AfterChallengeComplete);
    oracle->AddDialogue(PlayerDialogueState::AfterOracleAwakens, oracleTrialD);

    //------------ Oracle Awakens
    AreaTrigger *oracleAwakenTrigger = oracleObj.AddComponent<AreaTrigger>();
    DialoguePiece *oracleAwakenDP = new DialoguePiece(
        "You get your goosebumps\nas the man looks at you.\nThe wise man speaks\nvoice calm, yet echoing...", continueDO);
    Dialogue *oracleAwakenD = new Dialogue("Oracle Awakens", oracleAwakenDP, PlayerDialogueState::AfterOracleAwakens);
    oracleAwakenTrigger->SetDialogueToShow(oracleAwakenD);
    oracleAwakenTrigger->SetStateToInteract(PlayerDialogueState::AfterFirstAreaTrigger);

    //------------ First Area Trigger (Ruins Discovery)
    AreaTrigger *firstAreaTrigger = areaTriggerObj.AddComponent<AreaTrigger>();
    DialoguePiece *firstAreaDP = new DialoguePiece(
        "The air is heavy. The stone walls\nhum with a pulse not heard in ages.\nIn the center, there is a wise-looking man...\nStanding.", continueDO);
    Dialogue *firstAreaD = new Dialogue("First Area Trigger", firstAreaDP, PlayerDialogueState::AfterFirstAreaTrigger);
    firstAreaTrigger->SetDialogueToShow(firstAreaD);
    firstAreaTrigger->SetStateToInteract(PlayerDialogueState::Start);

    // - Strength: Spawn 3 enemies
    // - Wisdom: Trigger riddle UI
    // - Grace: Enable timed trap zone
    // IMPLEMENTATION ACTION NEEDED:
    // In dialogue system, hook these continueDO options to:
    // - SpawnEnemies()
    // - StartPuzzleRiddle()
    // - ActivateTrapZone()

    //------------ Oracle Unavailable Dialogue (fallback)
    DialoguePiece *unavailableDP = new DialoguePiece("I have nothing more to say to you.", leaveDO);
    Dialogue *unavailableD = new Dialogue("Unavailable Dialogue", unavailableDP, PlayerDialogueState::Null);
    oracle->SetUnavailableDialogue(unavailableD);
}

#pragma endregion
