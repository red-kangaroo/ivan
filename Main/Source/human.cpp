/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

/* Compiled through charsset.cpp */

#include "dbgmsgproj.h"
#include "whandler.h"
#include "devcons.h"

cint humanoid::DrawOrder[] =
{ TORSO_INDEX, GROIN_INDEX, RIGHT_LEG_INDEX, LEFT_LEG_INDEX, RIGHT_ARM_INDEX, LEFT_ARM_INDEX, HEAD_INDEX };

truth humanoid::BodyPartIsVital(int I) const { return I == TORSO_INDEX || I == HEAD_INDEX || I == GROIN_INDEX; }
truth humanoid::BodyPartCanBeSevered(int I) const { return I != TORSO_INDEX && I != GROIN_INDEX; }
int humanoid::OpenMultiplier() const { return HasAUsableArm() ? 1 : 3; }
int humanoid::CloseMultiplier() const { return HasAUsableArm() ? 1 : 2; }
int humanoid::GetCarryingStrength() const { return Max(GetAttribute(LEG_STRENGTH), 1) + CarryingBonus; }
void humanoid::CalculateBodyParts() { BodyParts = HUMANOID_BODYPARTS; }
void humanoid::CalculateAllowedWeaponSkillCategories() { AllowedWeaponSkillCategories = WEAPON_SKILL_CATEGORIES; }

v2 farmer::GetHeadBitmapPos() const { return v2(96, (4 + (RAND() & 1)) << 4); }
v2 farmer::GetRightArmBitmapPos() const { return v2(64, (RAND() & 1) << 4); }

void guard::SetWayPoints(const fearray<packv2>& What) { ArrayToVector(What, WayPoints); }

cchar* oree::FirstPersonBiteVerb() const { return "vomit acidous blood at"; }
cchar* oree::FirstPersonCriticalBiteVerb() const { return "vomit very acidous blood at"; }
cchar* oree::ThirdPersonBiteVerb() const { return "vomits acidous blood at"; }
cchar* oree::ThirdPersonCriticalBiteVerb() const { return "vomits very acidous blood at"; }
cchar* oree::BiteNoun() const { return "liquid"; }

truth skeleton::BodyPartIsVital(int I) const { return I == GROIN_INDEX || I == TORSO_INDEX; }

truth communist::ShowClassDescription() const { return GetAssignedName() != "Ivan"; }

v2 housewife::GetHeadBitmapPos() const { return v2(112, (RAND() % 6) << 4); }

truth zombie::BodyPartIsVital(int I) const { return I == GROIN_INDEX || I == TORSO_INDEX; }
festring zombie::GetZombieDescription() const { return Description; }

truth ghost::BodyPartIsVital(int I) const { return I == GROIN_INDEX || I == TORSO_INDEX; }
festring ghost::GetGhostDescription() const { return Description; }
cchar* ghost::FirstPersonUnarmedHitVerb() const { return "touch"; }
cchar* ghost::FirstPersonCriticalUnarmedHitVerb() const
{ return "critically touch"; }
cchar* ghost::ThirdPersonUnarmedHitVerb() const { return "touches"; }
cchar* ghost::ThirdPersonCriticalUnarmedHitVerb() const
{ return "critically touches"; }

truth angel::BodyPartIsVital(int I) const { return I == TORSO_INDEX || I == HEAD_INDEX; }

truth nihil::BodyPartIsVital(int I) const { return I == TORSO_INDEX || I == HEAD_INDEX; }

truth genie::BodyPartIsVital(int I) const { return I == TORSO_INDEX || I == HEAD_INDEX; }

material* golem::CreateBodyPartMaterial(int, long Volume) const { return MAKE_MATERIAL(GetConfig(), Volume); }

truth sumowrestler::EquipmentIsAllowed(int I) const { return I == BELT_INDEX; }

truth ghost::SpecialEnemySightedReaction(character*) { return !(Active = true); }

petrus::~petrus()
{
  game::SetPetrus(0);
}

truth ennerbeast::Hit(character* Enemy, v2, int, int)
{
  if(CheckIfTooScaredToHit(Enemy))
    return false;

  if(RAND() & 1)
    ADD_MESSAGE("%s yells: UGH UGHAaaa!", CHAR_DESCRIPTION(DEFINITE));
  else
    ADD_MESSAGE("%s yells: Uga Ugar Ugade Ugat!", CHAR_DESCRIPTION(DEFINITE));

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), GetPos(), 30);

  for(int x = Rect.X1; x <= Rect.X2; ++x)
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
    {
      int ScreamStrength = int(70 / (hypot(GetPos().X - x, GetPos().Y - y) + 1));

      if(ScreamStrength)
      {
        character* Char = GetNearSquare(x, y)->GetCharacter();

        if(Char && Char != this)
        {
          msgsystem::EnterBigMessageMode();

          if(Char->IsPlayer())
            ADD_MESSAGE("You are hit by the horrible waves of high sound.");
          else if(Char->CanBeSeenByPlayer())
            ADD_MESSAGE("%s is hit by the horrible waves of high sound.", Char->CHAR_NAME(DEFINITE));

          Char->ReceiveDamage(this, ScreamStrength, SOUND, ALL, YOURSELF, true);
          Char->CheckDeath(CONST_S("killed @bkp scream"), this);
          msgsystem::LeaveBigMessageMode();
        }

        GetNearLSquare(x, y)->GetStack()->ReceiveDamage(this, ScreamStrength, SOUND);
      }
    }

  EditNP(-100);
  EditAP(-1000000 / GetCWeaponSkill(BITE)->GetBonus());
  EditStamina(-1000, false);
  return true;
}

truth ennerchild::Hit(character* Enemy, v2, int, int)
{
  if(CheckIfTooScaredToHit(Enemy))
    return false;

  if(RAND() & 1)
    ADD_MESSAGE("%s yells: UGH UGHAaaa!", CHAR_DESCRIPTION(DEFINITE));
  else
    ADD_MESSAGE("%s yells: Uga Ugar Ugade Ugat!", CHAR_DESCRIPTION(DEFINITE));

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), GetPos(), 30);

  // Enner girl is older
  int BaseScreamStrength = 50;

  if(GetConfig() == BOY)
    BaseScreamStrength = 40;

  for(int x = Rect.X1; x <= Rect.X2; ++x)
    for(int y = Rect.Y1; y <= Rect.Y2; ++y)
    {
      int ScreamStrength = int(BaseScreamStrength / (hypot(GetPos().X - x, GetPos().Y - y) + 1));

      if(ScreamStrength)
      {
        character* Char = GetNearSquare(x, y)->GetCharacter();

        if(Char && Char != this)
        {
          msgsystem::EnterBigMessageMode();

          if(Char->IsPlayer())
            ADD_MESSAGE("You are hit by the horrible waves of high sound.");
          else if(Char->CanBeSeenByPlayer())
            ADD_MESSAGE("%s is hit by the horrible waves of high sound.", Char->CHAR_NAME(DEFINITE));

          Char->ReceiveDamage(this, ScreamStrength, SOUND, ALL, YOURSELF, true);
          Char->CheckDeath(CONST_S("killed @bkp scream"), this);
          msgsystem::LeaveBigMessageMode();
        }

        GetNearLSquare(x, y)->GetStack()->ReceiveDamage(this, ScreamStrength, SOUND);
      }
    }

  EditNP(-100);
  EditAP(-1000000 / GetCWeaponSkill(BITE)->GetBonus());
  EditStamina(-1000, false);
  return true;
}

truth ennerchild::ReceiveDamage(character* Damager, int Damage, int Type, int TargetFlags, int Direction,
                              truth Divide, truth PenetrateArmor, truth Critical, truth ShowMsg)
{
  truth Success = false;

  if(Type != SOUND)
  {
    Success = humanoid::ReceiveDamage(Damager, Damage, Type, TargetFlags, Direction,
                                          Divide, PenetrateArmor, Critical, ShowMsg);
  }

  return Success;
}

void skeleton::CreateCorpse(lsquare* Square)
{
  if(GetHead())
  {
    item* Skull = SevereBodyPart(HEAD_INDEX, false, Square->GetStack());
    Square->AddItem(Skull);
  }

  int Amount = 2 + (RAND() & 3);

  for(int c = 0; c < Amount; ++c)
    Square->AddItem(bone::Spawn());

  SendToHell();
}

void petrus::CreateCorpse(lsquare* Square)
{
  if(game::GetGloomyCaveStoryState() == 2)
    game::GetTeam(ATTNAM_TEAM)->SetRelation(game::GetTeam(PLAYER_TEAM), FRIEND);

  Square->AddItem(leftnutofpetrus::Spawn());
  SendToHell();
}

void humanoid::Save(outputfile& SaveFile) const
{
  character::Save(SaveFile);
  SaveFile << SWeaponSkill;
}

void humanoid::Load(inputfile& SaveFile)
{
  character::Load(SaveFile);
  SaveFile >> SWeaponSkill;

  if(GetRightWielded())
    for(sweaponskill* p : SWeaponSkill)
      if(p->IsSkillOf(GetRightWielded()))
      {
        SetCurrentRightSWeaponSkill(p);
        break;
      }

  if(GetLeftWielded())
    for(sweaponskill* p : SWeaponSkill)
      if(p->IsSkillOf(GetLeftWielded()))
      {
        SetCurrentLeftSWeaponSkill(p);
        break;
      }
}

truth golem::MoveRandomly()
{
  if(!(RAND() % 500))
  {
    Engrave(CONST_S("Golem Needs Master"));
    EditAP(-1000);
    return true;
  }
  else
    return character::MoveRandomly();
}

void ennerbeast::GetAICommand()
{
  SeekLeader(GetLeader());

  if(StateIsActivated(PANIC) || !(RAND() % 3))
    Hit(0, ZERO_V2, YOURSELF);

  if(CheckForEnemies(false, false, true))
    return;

  if(FollowLeader(GetLeader()))
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void ennerchild::GetAICommand()
{
  SeekLeader(GetLeader());

  if(StateIsActivated(PANIC) || !(RAND() % 3))
    Hit(0, ZERO_V2, YOURSELF);

  if(CheckForEnemies(false, false, true))
    return;

  if(FollowLeader(GetLeader()))
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void petrus::GetAICommand()
{
  int Enemies = 0;

  for(int c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
      Enemies += game::GetTeam(c)->GetEnabledMembers();

  if(Enemies && !RAND_N(250 / Min(Enemies, 50)))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s shouts a magnificent prayer to Valpurus.", CHAR_NAME(DEFINITE));

    angel* Angel = angel::Spawn(VALPURUS);
    Angel->SetLifeExpectancy(10000, 0);
    v2 Where = GetLevel()->GetNearestFreeSquare(Angel, GetPos());

    if(Where == ERROR_V2)
      Where = GetLevel()->GetRandomSquare(Angel);

    Angel->SetTeam(GetTeam());
    Angel->PutTo(Where);

    if(Angel->CanBeSeenByPlayer())
      ADD_MESSAGE("%s materializes.", Angel->CHAR_NAME(INDEFINITE));

    EditAP(-1000);
    return;
  }

  if(HP << 1 < MaxHP && (GetPos() - v2(28, 20)).GetLengthSquare() > 400 && !RAND_N(10))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

    GetLevel()->GetLSquare(28, 20)->KickAnyoneStandingHereAway();
    Move(v2(28, 20), true);
    EditAP(-1000);
    return;
  }

  if(!LastHealed || game::GetTick() - LastHealed > 16384)
    for(int d = 0; d < GetNeighbourSquares(); ++d)
    {
      square* Square = GetNeighbourSquare(d);

      if(Square)
      {
        character* Char = Square->GetCharacter();

        if(Char && GetRelation(Char) == FRIEND && HealFully(Char))
          return;
      }
    }

  StandIdleAI();
}

truth petrus::HealFully(character* ToBeHealed)
{
  truth DidSomething = false;

  for(int c = 0; c < ToBeHealed->GetBodyParts(); ++c)
    if(!ToBeHealed->GetBodyPart(c))
    {
      bodypart* BodyPart = 0;

      for(ulong ID : ToBeHealed->GetOriginalBodyPartID(c))
      {
        BodyPart = static_cast<bodypart*>(ToBeHealed->SearchForItem(ID));

        if(BodyPart)
          break;
      }

      if(!BodyPart || !BodyPart->CanRegenerate())
        continue;

      BodyPart->RemoveFromSlot();
      ToBeHealed->AttachBodyPart(BodyPart);
      BodyPart->RestoreHP();
      DidSomething = true;

      if(ToBeHealed->IsPlayer())
        ADD_MESSAGE("%s attaches your old %s back and heals it.",
                    CHAR_NAME(DEFINITE), BodyPart->GetBodyPartName().CStr());
      else if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s attaches the old %s of %s back and heals it.",
                    CHAR_NAME(DEFINITE), BodyPart->GetBodyPartName().CStr(), ToBeHealed->CHAR_DESCRIPTION(DEFINITE));
    }

  if(ToBeHealed->IsInBadCondition())
  {
    ToBeHealed->RestoreLivingHP();
    DidSomething = true;

    if(ToBeHealed->IsPlayer())
      ADD_MESSAGE("%s heals you fully.", CHAR_DESCRIPTION(DEFINITE));
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s heals %s fully.", CHAR_DESCRIPTION(DEFINITE), ToBeHealed->CHAR_DESCRIPTION(DEFINITE));
  }

  if(ToBeHealed->TemporaryStateIsActivated(POISONED))
  {
    ToBeHealed->DeActivateTemporaryState(POISONED);
    DidSomething = true;

    if(ToBeHealed->IsPlayer())
      ADD_MESSAGE("%s removes the foul poison in your body.", CHAR_DESCRIPTION(DEFINITE));
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s removes the foul poison in %s's body.",
                  CHAR_DESCRIPTION(DEFINITE), ToBeHealed->CHAR_DESCRIPTION(DEFINITE));
  }

  if(DidSomething)
  {
    LastHealed = game::GetTick();
    DexterityAction(10);
    return true;
  }
  else
    return false;
}

void petrus::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << LastHealed;
}

void petrus::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> LastHealed;
  game::SetPetrus(this);
}

item* humanoid::GetMainWielded() const
{
  if(GetMainArm())
    if(GetMainArm()->GetWielded())
      return GetMainArm()->GetWielded();
    else
      if(GetSecondaryArm())
        return GetSecondaryArm()->GetWielded();
      else
        return 0;
  else
    if(GetSecondaryArm())
      return GetSecondaryArm()->GetWielded();
    else
      return 0;
}

item* humanoid::GetSecondaryWielded() const
{
  if(GetMainArm() && GetMainArm()->GetWielded() && GetSecondaryArm())
    return GetSecondaryArm()->GetWielded();
  else
    return 0;
}

truth humanoid::Hit(character* Enemy, v2 HitPos, int Direction, int Flags)
{
  if(CheckIfTooScaredToHit(Enemy))
    return false;

  if(IsPlayer())
  {
    if(!(Enemy->IsMasochist() && GetRelation(Enemy) == FRIEND) && GetRelation(Enemy) != HOSTILE
       && !game::TruthQuestion(CONST_S("This might cause a hostile reaction. Are you sure? [y/N]")))
      return false;
  }
  else if(GetAttribute(WISDOM) >= Enemy->GetAttackWisdomLimit())
    return false;

  if(GetBurdenState() == OVER_LOADED)
  {
    if(IsPlayer())
      ADD_MESSAGE("You cannot fight while carrying so much.");

    return false;
  }

  int c, AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c))
      ++AttackStyles;

  int Chosen = RAND() % AttackStyles;

  for(c = 0, AttackStyles = 0; c < 8; ++c)
    if(GetAttackStyle() & (1 << c) && AttackStyles++ == Chosen)
    {
      Chosen = 1 << c;
      break;
    }

  if(StateIsActivated(VAMPIRISM) && !(RAND() % 2))
    {
      if(Chosen == USE_ARMS && CanAttackWithAnArm())
        if(!GetRightWielded() && !GetLeftWielded())
          Chosen = USE_HEAD;

      if(Chosen == USE_LEGS && HasTwoUsableLegs())
        Chosen = USE_HEAD;
    }

  switch(Chosen)
  {
   case USE_ARMS:
    if(CanAttackWithAnArm() && (!(Flags & SADIST_HIT) || HasSadistWeapon()))
    {
      msgsystem::EnterBigMessageMode();
      Hostility(Enemy);
      long FirstAPCost = 0, SecondAPCost = 0;
      arm* FirstArm, * SecondArm;

      if(RAND() & 1)
      {
        FirstArm = GetRightArm();
        SecondArm = GetLeftArm();
      }
      else
      {
        FirstArm = GetLeftArm();
        SecondArm = GetRightArm();
      }

      int Strength = Max(GetAttribute(ARM_STRENGTH), 1);

      if(FirstArm && FirstArm->GetDamage() && (!(Flags & SADIST_HIT) || FirstArm->HasSadistWeapon()))
      {
        FirstAPCost = FirstArm->GetAPCost();
        FirstArm->Hit(Enemy, HitPos, Direction, Flags);

        if(StateIsActivated(LEPROSY) && !RAND_N(25 * GetAttribute(ENDURANCE)))
          DropBodyPart(FirstArm->GetBodyPartIndex());
      }

      if(!GetAction() && IsEnabled() && Enemy->IsEnabled() && SecondArm && SecondArm->GetDamage()
         && (!(Flags & SADIST_HIT) || SecondArm->HasSadistWeapon()))
      {
        SecondAPCost = SecondArm->GetAPCost();
        SecondArm->Hit(Enemy, HitPos, Direction, Flags);

        if(StateIsActivated(LEPROSY) && !RAND_N(25 * GetAttribute(ENDURANCE)))
          DropBodyPart(SecondArm->GetBodyPartIndex());
      }

      EditNP(-50);
      EditAP(-Max(FirstAPCost, SecondAPCost));
      EditStamina(GetAdjustedStaminaCost(-1000, Strength), false);
      msgsystem::LeaveBigMessageMode();
      return true;
    }
   case USE_LEGS:
    if(HasTwoUsableLegs())
    {
      msgsystem::EnterBigMessageMode();
      Hostility(Enemy);
      Kick(GetNearLSquare(HitPos), Direction, Flags & SADIST_HIT);

      if(StateIsActivated(LEPROSY) && !RAND_N(25 * GetAttribute(ENDURANCE)))
        DropBodyPart(RAND_2 ? RIGHT_LEG_INDEX : LEFT_LEG_INDEX);

      msgsystem::LeaveBigMessageMode();
      return true;
    }
   case USE_HEAD:
    if(GetHead())
    {
      msgsystem::EnterBigMessageMode();
      Hostility(Enemy);
      Bite(Enemy, HitPos, Direction, Flags & SADIST_HIT);
      msgsystem::LeaveBigMessageMode();
      return true;
    }
   default:
    if(IsPlayer())
      ADD_MESSAGE("You are currently quite unable to damage anything.");

    return false;
  }
}

truth humanoid::AddSpecialSkillInfo(felist& List) const
{
  truth Something = false;

  if(CurrentRightSWeaponSkill && CurrentRightSWeaponSkill->GetHits() / 100)
  {
    List.AddEntry(CONST_S(""), LIGHT_GRAY);
    festring Buffer = CONST_S("right accustomization");
    Buffer.Resize(30);
    Buffer << CurrentRightSWeaponSkill->GetLevel();
    Buffer.Resize(40);
    Buffer << CurrentRightSWeaponSkill->GetHits() / 100;
    Buffer.Resize(50);

    if(CurrentRightSWeaponSkill->GetLevel() != 20)
      Buffer << (CurrentRightSWeaponSkill->GetLevelMap(CurrentRightSWeaponSkill->GetLevel() + 1)
                 - CurrentRightSWeaponSkill->GetHits()) / 100;
    else
      Buffer << '-';

    Buffer.Resize(60);
    int Bonus = CurrentRightSWeaponSkill->GetBonus();
    Buffer << '+' << (Bonus - 1000) / 10;

    if(Bonus %= 10)
      Buffer << '.' << Bonus;

    Buffer << '%';
    List.AddEntry(Buffer, WHITE);
    Something = true;
  }

  if(CurrentLeftSWeaponSkill && CurrentLeftSWeaponSkill->GetHits() / 100)
  {
    if(!Something)
      List.AddEntry(CONST_S(""), LIGHT_GRAY);

    festring Buffer = CONST_S("left accustomization");
    Buffer.Resize(30);
    Buffer << CurrentLeftSWeaponSkill->GetLevel();
    Buffer.Resize(40);
    Buffer << CurrentLeftSWeaponSkill->GetHits() / 100;
    Buffer.Resize(50);

    if(CurrentLeftSWeaponSkill->GetLevel() != 20)
      Buffer << (CurrentLeftSWeaponSkill->GetLevelMap(CurrentLeftSWeaponSkill->GetLevel() + 1)
                 - CurrentLeftSWeaponSkill->GetHits()) / 100;
    else
      Buffer << '-';

    Buffer.Resize(60);
    int Bonus = CurrentLeftSWeaponSkill->GetBonus();
    Buffer << '+' << (Bonus - 1000) / 10;

    if(Bonus %= 10)
      Buffer << '.' << Bonus;

    Buffer << '%';
    List.AddEntry(Buffer, WHITE);
    Something = true;
  }

  return Something;
}

void petrus::BeTalkedTo()
{
  if(!GetPos().IsAdjacent(PLAYER->GetPos()))
    return;

  if(GetRelation(PLAYER) == HOSTILE)
  {
    ADD_MESSAGE("Heretic! Dev/null is a place not worthy to receive thee!");
    return;
  }

  if(PLAYER->HasMuramasa() && PLAYER->HasMasamune())
  {
    if(game::TruthQuestion(CONST_S("Report your actions in the kingdom of Aslona? [y/N]"), REQUIRES_ANSWER))
    {
      game::PlayVictoryMusic();
      game::TextScreen(CONST_S("\"Yes, citizen? Ah, it is thee. I was wondering were hast thou wandered off. Art thou\n"
                               "coming back to beg for forgiveness and mercy?\"\n\n"
                               "But Petrus' anger is quickly quelled when you fall to your knees and present to him\n"
                               "the two regal swords of Aslona. He takes them and admires them in silence for a while,\n"
                               "though interrupted by Sir Lancelyn who realizes your deeds, screams in rage and is\n"
                               "promptly dragged away to a prison cell.\n\n"
                               "Lost in thoughts, Petrus hands the swords to a servant and turns back to you again:\n\n"
                               "\"I have decided thy fate, slave. I shan't have thee executed if thou canst prove thy worth.\n"
                               "Sir Galladon hast telepathically informed me that our trainee guards are just about ready\n"
                               "for their first war. Thou shalt join my army and help conquer Aslona while they are reeling.\n"
                               "For the glory of Valpurus!\"\n\nYou are victorious!"));

      game::GetCurrentArea()->SendNewDrawRequest();
      game::DrawEverything();
      PLAYER->ShowAdventureInfo();
      festring Msg = CONST_S("became an officer of the Attnamese army");
      AddScoreEntry(Msg, 3, false);
      game::End(Msg);
      return;
    }
  }

  if(PLAYER->HasGoldenEagleShirt())
  {
    ADD_MESSAGE("Petrus smiles. \"Thou hast defeated Oree! Mayst thou be blessed by Valpurus for the rest of thy life! "
                "And thou possess the Shirt of the Golden Eagle, the symbol of Our status! Return it now, please.\"");

    if(game::TruthQuestion(CONST_S("Will you give the Shirt of the Golden Eagle to Petrus? [y/n]"), REQUIRES_ANSWER))
    {
      game::PlayVictoryMusic();
      game::TextScreen(CONST_S("The Holy Shirt is returned to its old owner and you kneel down to receive your reward.\n"
                               "Petrus taps your shoulder with the Justifier and raises you to nobility. Later you\n"
                               "receive a small dukedom in the middle of tundra where you rule with justice till\n"
                               "the end of your content life.\n\nYou are victorious!"));

      game::GetCurrentArea()->SendNewDrawRequest();
      game::DrawEverything();
      PLAYER->ShowAdventureInfo();
      festring Msg = CONST_S("retrieved the Shirt of the Golden Eagle and was raised to nobility");
      AddScoreEntry(Msg, 4, false);
      game::End(Msg);
      return;
    }
    else
    {
      ADD_MESSAGE("Petrus's face turns red. \"I see. Thy greed hath overcome thy wisdom. Then, "
                  "we shall fight for the shiny shirt. May Valpurus bless him who is better.\"");

      /* And now we actually make his face change color ;-) */

      GetHead()->GetMainMaterial()->SetSkinColor(MakeRGB16(255, 75, 50));
      GetHead()->UpdatePictures();
      SendNewDrawRequest();
      game::AskForKeyPress(CONST_S("You are attacked! [press any key to continue]"));
      PLAYER->GetTeam()->Hostility(GetTeam());
      game::SetGloomyCaveStoryState(2);
      return;
    }
  }

  if(PLAYER->HasHeadOfElpuri())
  {
    game::PlayVictoryMusic();
    game::TextScreen(CONST_S("You have slain Elpuri, and Petrus grants you the freedom you desire.\n"
                             "You spend the next months in Attnam as an honored hero and when the\n"
                             "sea finally melts, you board the first ship, leaving your past forever\n"
                             "behind.\n\nYou are victorious!"));

    game::GetCurrentArea()->SendNewDrawRequest();
    game::DrawEverything();
    PLAYER->ShowAdventureInfo();
    festring Msg = CONST_S("defeated Elpuri and continued to further adventures");
    AddScoreEntry(Msg, 2, false);
    game::End(Msg);
  }
  else if(game::GetGloomyCaveStoryState() == 1)
  {
    ADD_MESSAGE("Petrus says: \"Bring me the head of Elpuri and we'll talk.\"");
    return;
  }

  if(!game::GetStoryState())
  {
    if(PLAYER->RemoveEncryptedScroll())
    {
      game::TextScreen(CONST_S("You kneel down and bow before the high priest and hand him the encrypted scroll.\n"
                               "Petrus raises his arm, the scroll glows yellow, and lo! The letters are clear and\n"
                               "readable. Petrus asks you to voice them aloud. The first two thousand words praise\n"
                               "Valpurus the Creator and all His manifestations and are followed by a canticle of\n"
                               "Saint Petrus the Lion-Hearted lasting roughly three thousand words. Finally there\n"
                               "are some sentences actually concerning your mission:\n\n"
                               "\"Alas, I fear dirty tongues have spread lies to my Lord's ears. I assure all tales\n"
                               "of treasures here in New Attnam are but mythic legends. There is nothing of value here.\n"
                               "The taxes are already an unbearable burden and I can't possibly pay more. However I do\n"
                               "not question the wisdom of the government's decisions. I will contribute what I can:\n"
                               "the ostriches will deliver an extra 10000 bananas to the capital and additionally the\n"
                               "slave that brought the message will henceforth be at Your disposal. I am certain this\n"
                               "satisfies the crown's needs.\"\n\n"
                               "\"Yours sincerely,\n"
                               "Richel Decos, the viceroy of New Attnam\""));

      game::TextScreen(CONST_S("You almost expected the last bit. Petrus seems to be deep in his thoughts and you\n"
                               "wonder what shape your destiny is taking in his mind. Suddenly he seems to return\n"
                               "to this reality and talks to you.\n\n"
                               "\"Oh, thou art still here. We were just discussing telepathically with Sir Galladon.\n"
                               "We started doubting Decos's alleged poverty a while back when he bought a couple of\n"
                               "medium-sized castles nearby. Thy brethren from New Attnam have also told Us about\n"
                               "vast riches seized from them. Our law says all such stolen valuables belong to \n"
                               "the Cathedral's treasury, so this is a severe claim. However, proof is needed,\n"
                               "and even if such was provided, We couldn't send soldiers over the snow fields\n"
                               "ere spring.\"\n\n"
                               "\"However, since thou now servest Us, We ought to find thee something to do. In Our\n"
                               "immesurable kindness and generosity, we giveth thee some time to see this majestic\n"
                               "city of Ours and marvel at its glory. Return to Us once thou hast admired Our\n"
                               "monumental Cathedral enough, and we shall have decided thy fate by then.\""));

      GetArea()->SendNewDrawRequest();
      ADD_MESSAGE("\"The patrol guard will watch out for you, so don't thee think of running away.\"");
      game::SetStoryState(1);
    }
    else
      ADD_MESSAGE("\"Yes, citizen? We are quite busy now, thou shalt not disturb Us without proper cause.\"");
  }
  else if(game::GetStoryState() == 1)
  {
    game::TextScreen(CONST_S("\"Ah, here thou art, my brave warrior savage! Our agents hast informed Us that they\n"
                             "witnessed thou leaving the dreaded underwater tunnel. This means thou most likely hast\n"
                             "defeated Genetrix Vesana and art a talented monster slayer. We happen to have a task\n"
                             "perfect for such a person.\"\n\n"
                             "\"An evil dark frog named Elpuri who hates Valpurus and Attnam more than anything hath\n"
                             "taken control over an abandoned mine nearby. It is pestering our fine city in many ways\n"
                             "and reconnaissance has reported an army of monsters gathering in the cave. Our guards\n"
                             "are not trained to fight underground and We dare not send them. To make things worse,\n"
                             "someone hath recently stolen from Us the greatest armor in existence - the Shirt of the Golden Eagle.\n"
                             "Elpuri cannot wear it but he who can is now nearly immortal.\"\n\n"
                             "\"We have marked the location of the gloomy cave on thy map. We want you to dive\n"
                             "into it and slay the vile frog. Bring Us its head and We reward thee with freedom.\n"
                             "Shouldst thou also find the Shirt, We'll knight thee.\"\n\n"
                             "\"Good luck, and return when thou hast succeeded.\""));

    game::LoadWorldMap();
    v2 ElpuriCavePos = game::GetWorldMap()->GetEntryPos(0, ELPURI_CAVE);
    game::GetWorldMap()->GetWSquare(ElpuriCavePos)->ChangeOWTerrain(elpuricave::Spawn());
    game::GetWorldMap()->RevealEnvironment(ElpuriCavePos, 1);
    game::SaveWorldMap();
    GetArea()->SendNewDrawRequest();
    ADD_MESSAGE("\"And by the way, visit the librarian. He might have advice for thee.\"");
    game::SetGloomyCaveStoryState(1);
    game::SetStoryState(2);
  }
  else // StoryState == 2
    ADD_MESSAGE("\"Yes, citizen? We are quite busy now, thou shalt not disturb Us without proper cause.\"");
}

void priest::BeTalkedTo()
{
  if(GetRelation(PLAYER) == HOSTILE)
  {
    character::BeTalkedTo();
    return;
  }
  else if(!GetPos().IsAdjacent(PLAYER->GetPos()))
  {
    static long Said;

    if(GetConfig() != SILVA)
      humanoid::BeTalkedTo();
    else if(!game::TweraifIsFree())
      ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, 4)]);
    else
      ProcessAndAddMessage(GetFriendlyReplies()[4 + RandomizeReply(Said, 3)]);

    return;
  }

  if(PLAYER->IsBurning())
  {
    long Price = GetConfig() == VALPURUS ? 25 : 5;

    if(PLAYER->GetMoney() >= Price)
    {
      ADD_MESSAGE("\"Good %s, you're on fire! Quick, hand over %ld gold!\"", GetMasterGod()->GetName(), Price);

      if(game::TruthQuestion(CONST_S("Do you agree? [y/N]")))
      {
        PLAYER->Extinguish(true);
        PLAYER->SetMoney(PLAYER->GetMoney() - Price);
        SetMoney(GetMoney() + Price);
        return;
      }
    }
    else
      ADD_MESSAGE("\"Good %s, you're on fire! Quick, go find %ld gold so that I can help!\"", GetMasterGod()->GetName(), Price);
  }

  for(int c = 0; c < PLAYER->GetBodyParts(); ++c)
  {
    if(!PLAYER->GetBodyPart(c) && PLAYER->CanCreateBodyPart(c))
    {
      truth HasOld = false;

      for(ulong ID : PLAYER->GetOriginalBodyPartID(c))
      {
        bodypart* OldBodyPart = static_cast<bodypart*>(PLAYER->SearchForItem(ID));

        if(OldBodyPart)
        {
          HasOld = true;
          long Price = GetConfig() == VALPURUS ? 50 : 10;

          if(PLAYER->GetMoney() >= Price)
          {
            if(!OldBodyPart->CanRegenerate())
              ADD_MESSAGE("\"Sorry, I cannot put back bodyparts made of %s, not even your severed %s.\"",
                          OldBodyPart->GetMainMaterial()->GetName(false, false).CStr(),
                          PLAYER->GetBodyPartName(c).CStr());
            else
            {
              ADD_MESSAGE("\"I could put your old %s back in exchange for %ld gold.\"",
                          PLAYER->GetBodyPartName(c).CStr(), Price);

              if(game::TruthQuestion(CONST_S("Do you agree? [y/N]")))
              {
                OldBodyPart->SetHP(1);
                PLAYER->SetMoney(PLAYER->GetMoney() - Price);
                SetMoney(GetMoney() + Price);
                OldBodyPart->RemoveFromSlot();
                PLAYER->AttachBodyPart(OldBodyPart);
                return;
              }
            }
          }
          else
            ADD_MESSAGE("\"Your %s is severed. Help yourself and get %ldgp and I'll help you too.\"",
                        PLAYER->GetBodyPartName(c).CStr(), Price);
        }
      }

      long Price = GetConfig() == VALPURUS ? 100 : 20;

      if(PLAYER->GetMoney() >= Price)
      {
        if(HasOld)
          ADD_MESSAGE("\"I could still summon up a new one for %ld gold.\"", Price);
        else
          ADD_MESSAGE("\"Since you don't seem to have your original %s with you, "
                      "I could summon up a new one for %ld gold.\"",
                      PLAYER->GetBodyPartName(c).CStr(), Price);

        if(game::TruthQuestion(CONST_S("Agreed? [y/N]")))
        {
          PLAYER->SetMoney(PLAYER->GetMoney() - Price);
          SetMoney(GetMoney() + Price);
          PLAYER->CreateBodyPart(c);
          PLAYER->GetBodyPart(c)->SetHP(1);
          return;
        }
      }
      else if(!HasOld)
        ADD_MESSAGE("\"You don't have your original %s with you. I could create you a new one, "
                    "but my Divine Employer is not a communist and you need %ldgp first.\"",
                    PLAYER->GetBodyPartName(c).CStr(), Price);
    }

    if(PLAYER->GetBodyPart(c))
    {
      bodypart* BodyPart = PLAYER->GetBodyPart(c);

      if(BodyPart->CanRegenerate() && BodyPart->IsBurnt())
      {
        long Price = GetConfig() == VALPURUS ? 25 : 5;

        if(PLAYER->GetMoney() >= Price)
        {
            ADD_MESSAGE("\"I could cure the burns on your %s in exchange for %ld gold.\"",
                        PLAYER->GetBodyPartName(c).CStr(), Price);

          if(game::TruthQuestion(CONST_S("Do you agree? [y/N]")))
          {
            BodyPart->ResetBurning();
            PLAYER->SetMoney(PLAYER->GetMoney() - Price);
            SetMoney(GetMoney() + Price);
            return;
          }
        }
        else
          ADD_MESSAGE("\"Your %s is burnt. Bring me %ld gold pieces and I'll make it all better.\"",
                      PLAYER->GetBodyPartName(c).CStr(), Price);
      }
      else if(!BodyPart->CanRegenerate() && BodyPart->IsBurnt())
        ADD_MESSAGE("\"Sorry, I cannot heal bodyparts made of %s, not even your burnt %s.\"",
                          BodyPart->GetMainMaterial()->GetName(false, false).CStr(),
                          PLAYER->GetBodyPartName(c).CStr());
    }
  }

  if(PLAYER->TemporaryStateIsActivated(POISONED))
  {
    long Price = GetConfig() == VALPURUS ? 25 : 5;

    if(PLAYER->GetMoney() >= Price)
    {
      ADD_MESSAGE("\"You seem to be rather ill. I could give you a "
                  "small dose of antidote for %ld gold pieces.\"", Price);

      if(game::TruthQuestion(CONST_S("Do you agree? [y/N]")))
      {
        ADD_MESSAGE("You feel better.");
        PLAYER->DeActivateTemporaryState(POISONED);
        PLAYER->SetMoney(PLAYER->GetMoney() - Price);
        SetMoney(GetMoney() + Price);
        return;
      }
    }
    else
      ADD_MESSAGE("\"You seem to be rather ill. Get %ld gold pieces and I'll fix that.\"", Price);
  }

  if(PLAYER->TemporaryStateIsActivated(LEPROSY))
  {
    long Price = GetConfig() == VALPURUS ? 100 : 20;

    if(PLAYER->GetMoney() >= Price)
    {
      ADD_MESSAGE("\"You seem to have contracted the vile disease of leprosy. "
                  "I could give you a small dose of medicine for %ld gold pieces.\"", Price);

      if(game::TruthQuestion(CONST_S("Do you agree? [y/N]")))
      {
        ADD_MESSAGE("You feel better.");
        PLAYER->DeActivateTemporaryState(LEPROSY);
        PLAYER->SetMoney(PLAYER->GetMoney() - Price);
        SetMoney(GetMoney() + Price);
        return;
      }
    }
    else
      ADD_MESSAGE("\"You seem to be falling apart. Get %ld gold pieces and I'll fix that.\"", Price);
  }

  if(PLAYER->TemporaryStateIsActivated(LYCANTHROPY))
  {
    long Price = GetConfig() == VALPURUS ? 100 : 20;

    if(PLAYER->GetMoney() >= Price)
    {
      ADD_MESSAGE("\"You seem to be turning into a werewolf quite frequently. Well, everyone has right to "
                  "little secret habits, but if you wish to donate %ldgp to %s, maybe I could pray to %s to "
                  "remove the canine blood from your veins, just so you don't scare our blessed youth.\"",
                  Price, GetMasterGod()->GetName(), GetMasterGod()->GetObjectPronoun());

      if(game::TruthQuestion(CONST_S("Do you agree? [y/N]")))
      {
        ADD_MESSAGE("You feel better.");
        PLAYER->DeActivateTemporaryState(LYCANTHROPY);
        PLAYER->SetMoney(PLAYER->GetMoney() - Price);
        SetMoney(GetMoney() + Price);
        return;
      }
    }
    else
      ADD_MESSAGE("\"You seem to be lycanthropic. I might be able to do something "
                  "for that but I need %ldgp for the ritual materials first.\"", Price);
  }

  if(PLAYER->TemporaryStateIsActivated(VAMPIRISM))
  {
    long Price = GetConfig() == VALPURUS ? 100 : 20;

    if(PLAYER->GetMoney() >= Price)
    {
      ADD_MESSAGE("\"You seem to have an addiction to drinking blood. Well, everyone has right to "
                  "little secret habits, but if you wish to donate %ldgp to %s, maybe I could pray "
                  "%s to remove your vampiric urges, just so you don't victimize our besotted youth.\""
                  , Price, GetMasterGod()->GetName(), GetMasterGod()->GetObjectPronoun());

      if(game::TruthQuestion(CONST_S("Do you agree? [y/N]")))
      {
        ADD_MESSAGE("You feel better.");
        PLAYER->DeActivateTemporaryState(VAMPIRISM);
        PLAYER->SetMoney(PLAYER->GetMoney() - Price);
        SetMoney(GetMoney() + Price);
        return;
      }
    }
    else
      ADD_MESSAGE("\"You seem to be vampiric. I might be able to do something for that but "
                  "I need %ldgp for the ritual materials first.\"", Price);
  }

  if(PLAYER->TemporaryStateIsActivated(PARASITE_TAPE_WORM))
  {
    long Price = GetConfig() == VALPURUS ? 100 : 20;

    if(PLAYER->GetMoney() >= Price)
    {
      ADD_MESSAGE("\"Ugh, there seems to be some other creature living in your body. I could try "
                  "to purge the parasite, that is if you wish to donate %ldgp to %s, of course.\""
                  , Price, GetMasterGod()->GetName(), GetMasterGod()->GetObjectPronoun());

      if(game::TruthQuestion(CONST_S("Do you agree? [y/N]")))
      {
        ADD_MESSAGE("You feel better.");
        PLAYER->DeActivateTemporaryState(PARASITE_TAPE_WORM);
        PLAYER->SetMoney(PLAYER->GetMoney() - Price);
        SetMoney(GetMoney() + Price);
        return;
      }
    }
    else
      ADD_MESSAGE("\"You seem to have an unwanted guest in your guts. I can help but "
                  "I need %ldgp for a ritual of cleansing.\"", Price);
  }

  if(PLAYER->TemporaryStateIsActivated(PARASITE_MIND_WORM))
  {
    long Price = GetConfig() == VALPURUS ? 100 : 20;

    if(PLAYER->GetMoney() >= Price)
    {
      ADD_MESSAGE("\"Ugh, there seems to be some other creature living in your body. I could try "
                  "to purge the parasite, that is if you wish to donate %ldgp to %s, of course.\""
                  , Price, GetMasterGod()->GetName(), GetMasterGod()->GetObjectPronoun());

      if(game::TruthQuestion(CONST_S("Do you agree? [y/N]")))
      {
        ADD_MESSAGE("You feel better.");
        PLAYER->DeActivateTemporaryState(PARASITE_MIND_WORM);
        PLAYER->SetMoney(PLAYER->GetMoney() - Price);
        SetMoney(GetMoney() + Price);
        return;
      }
    }
    else
      ADD_MESSAGE("\"You seem to have an unwanted guest in your head. I can help but "
                  "I need %ldgp for a ritual of cleansing.\"", Price);
  }

  static long Said;

  if(GetConfig() != SILVA)
    humanoid::BeTalkedTo();
  else if(!game::TweraifIsFree())
    ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, 4)]);
  else
    ProcessAndAddMessage(GetFriendlyReplies()[4 + RandomizeReply(Said, 3)]);
}

void skeleton::BeTalkedTo()
{
  if(GetHead())
    humanoid::BeTalkedTo();
  else if(GetPos().IsAdjacent(PLAYER->GetPos()))
    ADD_MESSAGE("The headless %s remains silent.", CHAR_DESCRIPTION(DEFINITE));
}

void communist::BeTalkedTo()
{
  if(GetRelation(PLAYER) != HOSTILE
     && GetTeam() != PLAYER->GetTeam()
     && PLAYER->GetRelativeDanger(this, true) > 0.1
     && GetPos().IsAdjacent(PLAYER->GetPos())
   )
  {
    ADD_MESSAGE("%s seems to be very friendly. \"%s make good communist. %s go with %s!\"",
                CHAR_DESCRIPTION(DEFINITE), PLAYER->GetAssignedName().CStr(),
                CHAR_NAME(UNARTICLED), PLAYER->GetAssignedName().CStr());

    for(character* Char : GetTeam()->GetMember())
    {
      if(Char != this)
        Char->ChangeTeam(PLAYER->GetTeam());

      if(GetTeam()->GetMembers() == 1) // Only Ivan left in Party
        break;
    }

    ChangeTeam(PLAYER->GetTeam());
  }
  else if(GetTeam() != PLAYER->GetTeam() && !(RAND() % 5))
    ADD_MESSAGE("\"You weak. Learn killing and come back.\"");
  else
    character::BeTalkedTo();
}

void hunter::BeTalkedTo()
{
  if(GetRelation(PLAYER) != HOSTILE && GetMainWielded() && !(RAND() % 10))
    ADD_MESSAGE("\"This is my %s. There are many like it but this one is mine. My %s is my best friend.\"",
                GetMainWielded()->CHAR_NAME(UNARTICLED), GetMainWielded()->CHAR_NAME(UNARTICLED));
  else
    character::BeTalkedTo();
}

void tourist::BeTalkedTo()
{
  if(GetConfig() == CHILD && GetPos().IsAdjacent(PLAYER->GetPos()))
  {
    character* Spider = 0;

    // check all enabled members of PLAYER_TEAM
    for(character* p : game::GetTeam(PLAYER_TEAM)->GetMember())
      if(p->IsEnabled() && !p->IsPlayer() && p->IsSpider()
         && (p->GetConfig() != LARGE && p->GetConfig() != GIANT)) // check for lobh-se first
        Spider = p;

    if(!Spider) // lobh-se not found
    {
      for(character* p : game::GetTeam(PLAYER_TEAM)->GetMember())
        if(p->IsEnabled() && !p->IsPlayer() && p->IsSpider()) // check for any spider
          Spider = p;
    }

    static long Said;

    if(GetRelation(PLAYER) == HOSTILE) // hostile response
    {
      ADD_MESSAGE("\"Daddy!!! Hit this man!!! He teases me!!!\"");
      return;
    }
    else if(Spider && !game::ChildTouristHasSpider()) // implement truthquestion + proper dialogue  // quest fulfilled
    {
      ADD_MESSAGE("\"Wow, what a cool spider!!! Can I have it mister? Can I?\"");
      festring GiveSpider = CONST_S("Will you give ") + Spider->CHAR_NAME(DEFINITE) +
                            CONST_S(" to ") + CHAR_NAME(DEFINITE) + CONST_S("? [y/N]");
      if(game::TruthQuestion(GiveSpider))
      {
        ADD_MESSAGE("\"Thanks a lot mister!!! Here, you can have this.\"");
        item* Reward = 0; // create gift item

        if(Spider->GetConfig() != LARGE && Spider->GetConfig() != GIANT) // must be lobh-se
        {
          Reward = scrollofwishing::Spawn();
        }
        else if(Spider->GetConfig() == LARGE || Spider->GetConfig() == GIANT) // other spider
        {
          Reward = stick::Spawn();
          Reward->InitMaterials(MAKE_MATERIAL(BALSA_WOOD)); // balsa stick
        }
        else
          ABORT("Man, this ain't my spider; this is a cell phone!");

        PLAYER->GetStack()->AddItem(Reward); // add gift to player's inventory
        ADD_MESSAGE("%s hands you %s.", CHAR_NAME(DEFINITE), Reward->CHAR_NAME(INDEFINITE));
        team* Team = game::GetTeam(TOURIST_TEAM);
        Spider->ChangeTeam(Team); // change spider to tourist team
        game::SetTouristHasSpider(); // sets game::TouristHasSpider to true
      }
      else
        ADD_MESSAGE("\"Aw... you're no fun!!!\"");
    }
    else if(!Spider && !game::ChildTouristHasSpider()) // kid does not have spider; normal chat
      ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, GetFriendlyReplies().Size)]);
    else if(game::ChildTouristHasSpider() && !(RAND() % 4))
      ADD_MESSAGE("\"My friends back home will be so jealous of my new pet spider!!!\"");
    else // kid has spider; normal chat (no spider request)
      ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, GetFriendlyReplies().Size - 1)]);
  }
  else // not child tourist; normal chat
    character::BeTalkedTo();
}

void slave::BeTalkedTo()
{
  if(GetRelation(PLAYER) == HOSTILE)
  {
    ADD_MESSAGE("\"Yikes!\"");
    return;
  }

  if(GetPos().IsAdjacent(PLAYER->GetPos()))
  {
    room* Room = GetHomeRoom();

    if(Room && Room->MasterIsActive())
    {
      character* Master = Room->GetMaster();

      if(PLAYER->GetMoney() >= 50)
      {
        ADD_MESSAGE("%s talks: \"Do you want to buy me? 50 gold pieces. "
                    "I work very hard.\"", CHAR_DESCRIPTION(DEFINITE));

        if(game::TruthQuestion(CONST_S("Do you want to buy him? [y/N]")))
        {
          PLAYER->SetMoney(PLAYER->GetMoney() - 50);
          Master->SetMoney(Master->GetMoney() + 50);
          ChangeTeam(PLAYER->GetTeam());
          RemoveHomeData();
        }
      }
      else
        ADD_MESSAGE("\"Don't touch me! Master doesn't want people to touch "
                    "sale items. I'm worth 50 gold pieces, you know!\"");

      return;
    }

    if(GetTeam() == PLAYER->GetTeam())
    {
      if((PLAYER->GetMainWielded() && PLAYER->GetMainWielded()->IsWhip()) ||
         (PLAYER->GetSecondaryWielded() && PLAYER->GetSecondaryWielded()->IsWhip()))
        ADD_MESSAGE("\"Don't hit me! I work! I obey! I don't think!\"");
      else
        character::BeTalkedTo();
    }
    else
      ADD_MESSAGE("\"I'm free! Rejoice!\"");
  }
  else
    character::BeTalkedTo();
}

void slave::GetAICommand()
{
  SeekLeader(GetLeader()); DBG2(GetNameSingular().CStr(), GetLeader());

  if(CheckAIZapOpportunity()){ DBG1(GetNameSingular().CStr());
    return;
  }

  if(CheckForEnemies(true, true, true)){ DBG1(GetNameSingular().CStr());
    return;
  }

  if(CheckForUsefulItemsOnGround()){ DBG1(GetNameSingular().CStr());
    return;
  }

  if(FollowLeader(GetLeader()))
    return;

  if(CheckForDoors())
    return;

  if(!GetHomeRoom() || !GetHomeRoom()->MasterIsActive())
  {
    RemoveHomeData();

    if(MoveRandomly())
      return;
  }
  else if(MoveTowardsHomePos())
    return;

  EditAP(-1000);
}

void librarian::BeTalkedTo()
{
  if(GetRelation(PLAYER) == HOSTILE)
  {
    ADD_MESSAGE("\"The pen is mightier than the sword! Fall, unlearned one!\"");
    return;
  }
  else if(!GetPos().IsAdjacent(PLAYER->GetPos()))
    return;

  // TODO: Replies for TX and Aslona!

  static long Said;

  switch(RandomizeReply(Said, 12))
  {
   case 0:
    if(game::GetPetrus() && !game::GetStoryState())
      ADD_MESSAGE("\"Thou shouldst visit Petrus in his great Cathedral.\"");
    else if(game::GetPetrus() && game::GetStoryState() == 1)
      ADD_MESSAGE("\"Thou shouldst visit Petrus if thou art in need of further adventures.\"");
    else
      ADD_MESSAGE("\"They say a wand of polymorph hath dozens of uses.\"");

    break;
   case 1:
    if(game::GetPetrus() && game::GetGloomyCaveStoryState())
      ADD_MESSAGE("\"Thou art going to fight Elpuri? Beware! It is a powerful enemy. Other monsters "
                  "are very vulnerable if surrounded by thy party, but not that beast, for it may "
                  "slay a horde of thy friends at once with its horrendous tail attack.\"");
    else if(game::GetXinrochTombStoryState())
      ADD_MESSAGE("\"Thou art going to delve into the Tomb of Xinroch? Beware, for it is a place of horrific darkness and abundant necromancy.\"");
    /*else if(game::GetAslonaStoryState())
      ADD_MESSAGE("\"Elpuri the Dark Frog abhors light and resides in a level of eternal darkness.\"");*/
    else
      ADD_MESSAGE("\"Thou shalt remember: Scientia est potentia.\"");

    break;
   case 2:
    if(game::GetPetrus() && game::GetGloomyCaveStoryState())
      ADD_MESSAGE("\"Elpuri the Dark Frog abhors light and resides in a level of eternal darkness.\"");
    else if(game::GetXinrochTombStoryState())
      ADD_MESSAGE("\"The Tomb of Xinroch is a chilling place. It is said that a whole cavern of magical ice can be found when wandering its tunnels.\"");
    /*else if(game::GetAslonaStoryState())
      ADD_MESSAGE("\"Elpuri the Dark Frog abhors light and resides in a level of eternal darkness.\"");*/
    else
      ADD_MESSAGE("\"Shh! Thou shalt be silent in the library.\"");

    break;
   case 3:
    if(game::GetPetrus() && game::GetGloomyCaveStoryState())
      ADD_MESSAGE("\"Elpuri's attacks are so strong that they may shatter many of thy precious items.\"");
    else if(game::GetXinrochTombStoryState())
      ADD_MESSAGE("\"The Tomb of Xinroch is guarded by fanatical dark knights that once followed Xinroch and swore to protect him even in death.\"");
    /*else if(game::GetAslonaStoryState())
      ADD_MESSAGE("\"Elpuri the Dark Frog abhors light and resides in a level of eternal darkness.\"");*/
    else
      ADD_MESSAGE("\"Dost thou not smell all the knowledge floating around here?\"");

    break;
   case 4:
    if(!RAND_2)
      ADD_MESSAGE("\"It is said that Loricatus, the god of smithing, can upgrade thy weapons' materials.\"");
    else
      ADD_MESSAGE("\"It is said that Atavus, the god of support and charity, may bolster thy defenses.\"");

    break;
   case 5:
    if(game::GetPetrus() && game::GetGloomyCaveStoryState())
      ADD_MESSAGE("\"The Shirt of the Golden Eagle is a legendary artifact. Thou canst not find a better armor.\"");
    /*else if(game::GetXinrochTombStoryState())
      ADD_MESSAGE("\"The Tomb of Xinroch is guarded by fanatical dark knights that once followed Xinroch and swore to protect him even in death.\"");
    else if(game::GetAslonaStoryState())
      ADD_MESSAGE("\"Elpuri the Dark Frog abhors light and resides in a level of eternal darkness.\"");*/
    else
      ADD_MESSAGE("\"In this book they talk about Mortifer, the great chaos god. He hates us "
                  "mortals more than anything and will respond only to Champions of Evil.\"");

    break;
   case 6:
    ADD_MESSAGE("\"Attnam is traditionally ruled by the high priest of the Great Frog. He "
                "holds the Shirt of the Golden Eagle and has always killed his predecessor.\"");
    break;
   case 7:
    ADD_MESSAGE("\"They say thou shouldst keep all the artifacts thou findst. "
                "They shall make thee famous after thy retirement.\"");
    break;
   case 8:
    ADD_MESSAGE("\"If thou wilt ever encounter an enner beast, know this: It is a horrible foe. "
                "It may shatter thy items and armor with its scream that penetrates iron and stone. "
                "Thou shouldst not engage it in melee but rather kill it from afar.\"");
    break;
   case 9:
    if(game::GetPetrus() && game::GetGloomyCaveStoryState())
      ADD_MESSAGE("\"Thou art not alone in thy attempt to defeat Elpuri. A brave "
                  "adventurer called Ivan also diveth into its cave not long ago.\"");
    /*else if(game::GetXinrochTombStoryState())
      ADD_MESSAGE("\"The Tomb of Xinroch is guarded by fanatical dark knights that once followed Xinroch and swore to protect him even in death.\"");
    else if(game::GetAslonaStoryState())
      ADD_MESSAGE("\"Elpuri the Dark Frog abhors light and resides in a level of eternal darkness.\"");*/
    else
      ADD_MESSAGE("\"It is said that chaotic gods offer great power to their followers. But thou "
                  "must remember: unlike lawfuls, they shall not help thee when things go bad.\"");

    break;
   case 10:
    if(!RAND_2)
      ADD_MESSAGE("\"If a man cannot choose, he ceases to be a man.\"");
    else
      ADD_MESSAGE("\"It is said that Cruentus, the god of bloodshed, may empower thy weapons.\"");

    break;
   case 11:
    ADD_MESSAGE("%s sighs: \"The censorship laws in this town are really too strict...\"",
                CHAR_DESCRIPTION(DEFINITE));
    break;
  }
}

truth communist::MoveRandomly()
{
  switch(RAND() % 1000)
  {
   case 0:
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s engraves something to the ground.", CHAR_NAME(UNARTICLED));

    Engrave(CONST_S("The bourgeois is a bourgeois -- for the benefit of the working class."));
    return true;
   case 1:
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s engraves something to the ground.", CHAR_NAME(UNARTICLED));

    Engrave(CONST_S("Proletarians of all countries, unite!"));
    return true;
   case 2:
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s engraves something to the ground.", CHAR_NAME(UNARTICLED));

    Engrave(CONST_S("Capital is therefore not only personal; it is a social power."));
    return true;
   default:
    return character::MoveRandomly();
  }
}

void zombie::BeTalkedTo()
{
  if(!HasHead() && GetPos().IsAdjacent(PLAYER->GetPos()))
  {
    ADD_MESSAGE("The headless %s remains silent.", CHAR_DESCRIPTION(DEFINITE));
  }
  else if(GetRelation(PLAYER) == HOSTILE && PLAYER->GetAttribute(INTELLIGENCE) > 5)
  {
    if(RAND() % 5)
    {
      if(GetHead())
        ADD_MESSAGE("\"Need brain!!\"");
      else
        ADD_MESSAGE("\"Need head with brain!!\"");
    }
    else
      ADD_MESSAGE("\"Redrum! Redrum! Redrum!\"");
  }
  else
    character::BeTalkedTo();
}

void angel::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << LastHealed;
}

void angel::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> LastHealed;
}

void angel::CreateInitialEquipment(int SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);
  GetStack()->AddItem(holybook::Spawn(GetConfig(), SpecialFlags));
  armor* Equipment;
  meleeweapon* Weapon;

  switch(GetMasterGod()->GetBasicAlignment())
  {
   case GOOD:
    Equipment = bodyarmor::Spawn(PLATE_MAIL, SpecialFlags|NO_MATERIALS);
    Equipment->InitMaterials(MAKE_MATERIAL(ANGEL_HAIR), !(SpecialFlags & NO_PIC_UPDATE));
    Equipment->SetEnchantment(1);
    SetBodyArmor(Equipment);
    Weapon = meleeweapon::Spawn(LONG_SWORD, SpecialFlags|NO_MATERIALS);
    Weapon->InitMaterials(MAKE_MATERIAL(MITHRIL), MAKE_MATERIAL(MITHRIL), !(SpecialFlags & NO_PIC_UPDATE));
    Weapon->SetEnchantment(2);
    SetRightWielded(Weapon);
    Equipment = shield::Spawn(0, SpecialFlags|NO_MATERIALS);
    Equipment->InitMaterials(MAKE_MATERIAL(MITHRIL), !(SpecialFlags & NO_PIC_UPDATE));
    Equipment->SetEnchantment(2);
    SetLeftWielded(Equipment);
    GetCWeaponSkill(LARGE_SWORDS)->AddHit(20000);
    GetCWeaponSkill(SHIELDS)->AddHit(50000);
    GetCurrentRightSWeaponSkill()->AddHit(20000);
    GetCurrentLeftSWeaponSkill()->AddHit(20000);
    GetRightArm()->SetDexterity(40);
    GetLeftArm()->SetDexterity(40);
    break;
   case NEUTRAL:
    Equipment = cloak::Spawn(0, SpecialFlags|NO_MATERIALS);
    Equipment->InitMaterials(MAKE_MATERIAL(ANGEL_HAIR), !(SpecialFlags & NO_PIC_UPDATE));
    Equipment->SetEnchantment(1);
    SetCloak(Equipment);
    Weapon = meleeweapon::Spawn(WAR_HAMMER, SpecialFlags|NO_MATERIALS);
    Weapon->InitMaterials(MAKE_MATERIAL(MITHRIL), MAKE_MATERIAL(TEAK_WOOD), !(SpecialFlags & NO_PIC_UPDATE));
    Weapon->SetEnchantment(2);
    SetRightWielded(Weapon);
    Weapon = meleeweapon::Spawn(WAR_HAMMER, SpecialFlags|NO_MATERIALS);
    Weapon->InitMaterials(MAKE_MATERIAL(MITHRIL), MAKE_MATERIAL(TEAK_WOOD), !(SpecialFlags & NO_PIC_UPDATE));
    Weapon->SetEnchantment(2);
    SetLeftWielded(Weapon);
    GetCWeaponSkill(BLUNT_WEAPONS)->AddHit(50000);
    GetCurrentRightSWeaponSkill()->AddHit(20000);
    GetCurrentLeftSWeaponSkill()->AddHit(20000);
    SetEndurance(40);
    break;
   case EVIL:
    Weapon = meleeweapon::Spawn(HALBERD, SpecialFlags|NO_MATERIALS);
    Weapon->InitMaterials(MAKE_MATERIAL(MITHRIL), MAKE_MATERIAL(EBONY_WOOD), !(SpecialFlags & NO_PIC_UPDATE));
    Weapon->SetEnchantment(2);
    SetRightWielded(Weapon);
    Equipment = gauntlet::Spawn(0, SpecialFlags|NO_MATERIALS);
    Equipment->InitMaterials(MAKE_MATERIAL(ANGEL_HAIR), !(SpecialFlags & NO_PIC_UPDATE));
    Equipment->SetEnchantment(1);
    SetRightGauntlet(Equipment);
    Equipment = gauntlet::Spawn(0, SpecialFlags|NO_MATERIALS);
    Equipment->InitMaterials(MAKE_MATERIAL(ANGEL_HAIR), !(SpecialFlags & NO_PIC_UPDATE));
    Equipment->SetEnchantment(1);
    SetLeftGauntlet(Equipment);
    GetCWeaponSkill(POLE_ARMS)->AddHit(100000);
    GetCurrentRightSWeaponSkill()->AddHit(100000);
    GetRightArm()->SetStrength(40);
    GetLeftArm()->SetStrength(40);
  }
}

void kamikazedwarf::CreateInitialEquipment(int SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);
  SetRightWielded(holybook::Spawn(GetConfig(), SpecialFlags));
  GetCWeaponSkill(UNCATEGORIZED)->AddHit(GetWSkillHits());
  GetCurrentRightSWeaponSkill()->AddHit(GetWSkillHits());
}

truth kamikazedwarf::Hit(character* Enemy, v2 HitPos, int Direction, int Flags)
{
  if(!IsPlayer())
  {
    itemvector KamikazeWeapon;
    sortdata SortData(KamikazeWeapon, this, false, &item::IsKamikazeWeapon);
    SortAllItems(SortData);

    if(!KamikazeWeapon.empty())
    {
      if(IsElite() && RAND() & 1)
        ADD_MESSAGE("%s shouts: \"This time I won't fail, O Great %s!\"",
                    CHAR_DESCRIPTION(DEFINITE), GetMasterGod()->GetName());
      else if(RAND() & 1)
        ADD_MESSAGE("%s shouts: \"For %s!\"",
                    CHAR_DESCRIPTION(DEFINITE), GetMasterGod()->GetName());
      else
        ADD_MESSAGE("%s screams: \"%s, here I come!\"",
                    CHAR_DESCRIPTION(DEFINITE), GetMasterGod()->GetName());

      if(KamikazeWeapon[RAND_N(KamikazeWeapon.size())]->Apply(this))
        return true;
    }
  }

  return humanoid::Hit(Enemy, HitPos, Direction, Flags);
}

void kamikazedwarf::GetAICommand()
{
  if(GetHomeRoom())
    StandIdleAI();
  else
  {
    if(!RAND_N(50))
    {
      SingRandomSong();
      return;
    }

    character::GetAICommand();
  }
}

int humanoid::GetSize() const
{
  int Size = 0;

  if(GetHead())
    Size += GetHead()->GetSize();

  if(GetTorso())
    Size += GetTorso()->GetSize();

  leg* RightLeg = GetRightLeg();
  leg* LeftLeg = GetLeftLeg();

  if(LeftLeg && RightLeg)
    Size += Max(LeftLeg->GetSize(), RightLeg->GetSize());
  else if(LeftLeg)
    Size += LeftLeg->GetSize();
  else if(RightLeg)
    Size += RightLeg->GetSize();

  return Size;
}

long humanoid::GetBodyPartSize(int I, int TotalSize) const
{
  switch(I)
  {
   case HEAD_INDEX: return 20;
   case TORSO_INDEX: return ((TotalSize - 20) << 1) / 5;
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX: return (TotalSize - 20) * 3 / 5;
   case GROIN_INDEX: return (TotalSize - 20) / 3;
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX: return (TotalSize - 20) * 3 / 5;
  }

  ABORT("Illegal humanoid bodypart size request!");
  return 0;
}

long humanoid::GetBodyPartVolume(int I) const
{
  switch(I)
  {
   case HEAD_INDEX: return 4000;
   case TORSO_INDEX: return (GetTotalVolume() - 4000) * 13 / 30;
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX: return (GetTotalVolume() - 4000) / 10;
   case GROIN_INDEX: return (GetTotalVolume() - 4000) / 10;
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX: return ((GetTotalVolume() - 4000) << 1) / 15;
  }

  ABORT("Illegal humanoid bodypart volume request!");
  return 0;
}

bodypart* humanoid::MakeBodyPart(int I) const
{
  switch(I)
  {
   case TORSO_INDEX: return humanoidtorso::Spawn(0, NO_MATERIALS);
   case HEAD_INDEX: return head::Spawn(0, NO_MATERIALS);
   case RIGHT_ARM_INDEX: return rightarm::Spawn(0, NO_MATERIALS);
   case LEFT_ARM_INDEX: return leftarm::Spawn(0, NO_MATERIALS);
   case GROIN_INDEX: return groin::Spawn(0, NO_MATERIALS);
   case RIGHT_LEG_INDEX: return rightleg::Spawn(0, NO_MATERIALS);
   case LEFT_LEG_INDEX: return leftleg::Spawn(0, NO_MATERIALS);
  }

  ABORT("Weird bodypart to make for a humanoid. It must be your fault!");
  return 0;
}

truth humanoid::ReceiveDamage(character* Damager, int Damage, int Type, int TargetFlags, int Direction,
                              truth Divide, truth PenetrateArmor, truth Critical, truth ShowMsg)
{
  int ChooseFrom[MAX_BODYPARTS], BodyParts = 0;

  if(TargetFlags & RIGHT_ARM && GetRightArm())
    ChooseFrom[BodyParts++] = 2;

  if(TargetFlags & LEFT_ARM && GetLeftArm())
    ChooseFrom[BodyParts++] = 3;

  if(TargetFlags & RIGHT_LEG && GetRightLeg())
    ChooseFrom[BodyParts++] = 5;

  if(TargetFlags & LEFT_LEG && GetLeftLeg())
    ChooseFrom[BodyParts++] = 6;

  if(TargetFlags & HEAD && GetHead())
    ChooseFrom[BodyParts++] = 1;

  if(TargetFlags & TORSO && GetTorso())
    ChooseFrom[BodyParts++] = 0;

  if(TargetFlags & GROIN && GetGroin())
    ChooseFrom[BodyParts++] = 4;

  if(!BodyParts)
    return false;

  truth Affected = false;

  if(Divide)
  {
    int c;
    long TotalVolume = 0;

    for(c = 0; c < BodyParts; ++c)
      TotalVolume += GetBodyPart(ChooseFrom[c])->GetBodyPartVolume();

    for(c = 0; c < BodyParts; ++c)
      if(ReceiveBodyPartDamage(Damager, long(Damage) * GetBodyPart(ChooseFrom[c])->GetBodyPartVolume() / TotalVolume,
                               Type, ChooseFrom[c], Direction, PenetrateArmor, Critical, false))
        Affected = true;
  }
  else
  {
    long Possibility[MAX_BODYPARTS], PossibilitySum = 0;
    int Index = 0;

    for(int c = 0; c < BodyParts; ++c)
      PossibilitySum += Possibility[Index++] = GetBodyPart(ChooseFrom[c])->GetBodyPartVolume();

    Index = femath::WeightedRand(Possibility, PossibilitySum);
    Affected = ReceiveBodyPartDamage(Damager, Damage, Type, ChooseFrom[Index],
                                     Direction, PenetrateArmor, Critical, false);
  }

  if(!Affected && ShowMsg)
  {
    if(IsPlayer())
      ADD_MESSAGE("You are not hurt.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s is not hurt.", GetPersonalPronoun().CStr());
  }

  if(DamageTypeAffectsInventory(Type))
  {
    for(int c = 0; c < GetEquipments(); ++c)
    {
      item* Equipment = GetEquipment(c);

      if(Equipment)
        Equipment->ReceiveDamage(Damager, Damage, Type);
    }

    GetStack()->ReceiveDamage(Damager, Damage, Type);
  }

  return Affected;
}

arm* humanoid::GetMainArm() const
{
  return GetRightArm() ? GetRightArm() : GetLeftArm();
}

arm* humanoid::GetSecondaryArm() const
{
  return GetRightArm() ? GetLeftArm() : 0;
}

cchar* humanoid::GetEquipmentName(int I) const // convert to array
{
  switch(I)
  {
   case HELMET_INDEX: return "helmet";
   case AMULET_INDEX: return "amulet";
   case CLOAK_INDEX: return "cloak";
   case BODY_ARMOR_INDEX: return "body armor";
   case BELT_INDEX: return "belt";
   case RIGHT_WIELDED_INDEX: return "right hand wielded";
   case LEFT_WIELDED_INDEX: return "left hand wielded";
   case RIGHT_RING_INDEX: return "right ring";
   case LEFT_RING_INDEX: return "left ring";
   case RIGHT_GAUNTLET_INDEX: return "right gauntlet";
   case LEFT_GAUNTLET_INDEX: return "left gauntlet";
   case RIGHT_BOOT_INDEX: return "right boot";
   case LEFT_BOOT_INDEX: return "left boot";
  }

  return "forbidden piece of cloth";
}

sorter humanoid::EquipmentSorter(int I) const
{
  switch(I)
  {
   case HELMET_INDEX: return &item::IsHelmet;
   case AMULET_INDEX: return &item::IsAmulet;
   case CLOAK_INDEX: return &item::IsCloak;
   case BODY_ARMOR_INDEX: return &item::IsBodyArmor;
   case BELT_INDEX: return &item::IsBelt;
   case RIGHT_WIELDED_INDEX:
   case LEFT_WIELDED_INDEX: return 0;
   case RIGHT_RING_INDEX:
   case LEFT_RING_INDEX: return &item::IsRing;
   case RIGHT_GAUNTLET_INDEX:
   case LEFT_GAUNTLET_INDEX: return &item::IsGauntlet;
   case RIGHT_BOOT_INDEX:
   case LEFT_BOOT_INDEX: return &item::IsBoot;
  }

  return 0;
}

bodypart* humanoid::GetBodyPartOfEquipment(int I) const
{
  switch(I)
  {
   case HELMET_INDEX:
   case AMULET_INDEX:
    return GetHead();
   case CLOAK_INDEX:
   case BODY_ARMOR_INDEX:
   case BELT_INDEX:
    return GetTorso();
   case RIGHT_WIELDED_INDEX:
   case RIGHT_RING_INDEX:
   case RIGHT_GAUNTLET_INDEX:
    return GetRightArm();
   case LEFT_WIELDED_INDEX:
   case LEFT_RING_INDEX:
   case LEFT_GAUNTLET_INDEX:
    return GetLeftArm();
   case RIGHT_BOOT_INDEX:
    return GetRightLeg();
   case LEFT_BOOT_INDEX:
    return GetLeftLeg();
  }

  return 0;
}

item* humanoid::GetEquipment(int I) const
{
  switch(I)
  {
   case HELMET_INDEX: return GetHelmet();
   case AMULET_INDEX: return GetAmulet();
   case CLOAK_INDEX: return GetCloak();
   case BODY_ARMOR_INDEX: return GetBodyArmor();
   case BELT_INDEX: return GetBelt();
   case RIGHT_WIELDED_INDEX: return GetRightWielded();
   case LEFT_WIELDED_INDEX: return GetLeftWielded();
   case RIGHT_RING_INDEX: return GetRightRing();
   case LEFT_RING_INDEX: return GetLeftRing();
   case RIGHT_GAUNTLET_INDEX: return GetRightGauntlet();
   case LEFT_GAUNTLET_INDEX: return GetLeftGauntlet();
   case RIGHT_BOOT_INDEX: return GetRightBoot();
   case LEFT_BOOT_INDEX: return GetLeftBoot();
  }

  return 0;
}

void humanoid::SetEquipment(int I, item* What)
{
  if(ivanconfig::GetRotateTimesPerSquare() > 0)
    if(What)What->ResetFlyingThrownStep();

  switch(I)
  {
   case HELMET_INDEX: SetHelmet(What); break;
   case AMULET_INDEX: SetAmulet(What); break;
   case CLOAK_INDEX: SetCloak(What); break;
   case BODY_ARMOR_INDEX: SetBodyArmor(What); break;
   case BELT_INDEX: SetBelt(What); break;
   case RIGHT_WIELDED_INDEX: SetRightWielded(What); break;
   case LEFT_WIELDED_INDEX: SetLeftWielded(What); break;
   case RIGHT_RING_INDEX: SetRightRing(What); break;
   case LEFT_RING_INDEX: SetLeftRing(What); break;
   case RIGHT_GAUNTLET_INDEX: SetRightGauntlet(What); break;
   case LEFT_GAUNTLET_INDEX: SetLeftGauntlet(What); break;
   case RIGHT_BOOT_INDEX: SetRightBoot(What); break;
   case LEFT_BOOT_INDEX: SetLeftBoot(What); break;
  }
}

truth humanoid::SwitchToCraft(recipedata rpd)
{DBGLN;
  craft* Craft = craft::Spawn(this);DBGLN;
  DBG4(rpd.GetTool(),rpd.GetTool2(),GetRightArm()?GetRightArm()->IsUsable():0,GetLeftArm()?GetLeftArm()->IsUsable():0);

  bool b1OK=false;
  bool b2OK=false;
  item* it;
  if(rpd.GetTool()){
    if(
      (GetRightArm() && GetRightArm()->IsUsable() && GetRightWielded() == rpd.GetTool()) ||
      (GetLeftArm()  && GetLeftArm()->IsUsable()  && GetLeftWielded()  == rpd.GetTool())
    ){
      b1OK=true;
      Craft->SetMoveCraftTool(false);
    }

    if(!b1OK && GetRightArm() && GetRightArm()->IsUsable()){
      if((it = GetRightWielded())){
        Craft->SetRightBackupID(it->GetID());
        it->MoveTo(GetStack());
      }

      rpd.GetTool()->RemoveFromSlot();
      SetRightWielded(rpd.GetTool());

      b1OK=true;
      Craft->SetMoveCraftTool(true);
    }

    if(!b1OK && GetLeftArm() && GetLeftArm()->IsUsable()){
      if((it = GetLeftWielded())){
        Craft->SetLeftBackupID(it->GetID());
        it->MoveTo(GetStack());
      }

      rpd.GetTool()->RemoveFromSlot();
      SetLeftWielded(rpd.GetTool());

      b1OK=true;
      Craft->SetMoveCraftTool(true);
    }

  }else{
    b1OK=true; //can craft somethings w/o tools
  }

  //TODO let the GetTool2() be equipped too?

  if(b1OK){
    Craft->SetCraftWhat(rpd);DBGLN;
    SetAction(Craft);DBGLN;
    return true;
  }

  ADD_MESSAGE("You have no usable arm.");
  rpd.SetAlreadyExplained();
  return false;
}

void humanoid::SwitchToDig(item* DigItem, v2 Square)
{
  if(IsPlayer())
    ADD_MESSAGE("You start digging.");

  dig* Dig = dig::Spawn(this);

  if(GetRightArm())
  {
    item* Item = GetRightArm()->GetWielded();

    if(Item && Item != DigItem)
    {
      Dig->SetRightBackupID(GetRightArm()->GetWielded()->GetID());
      GetRightArm()->GetWielded()->MoveTo(GetStack());
    }
  }

  if(GetLeftArm())
  {
    item* Item = GetLeftArm()->GetWielded();

    if(Item && Item != DigItem)
    {
      Dig->SetLeftBackupID(GetLeftArm()->GetWielded()->GetID());
      GetLeftArm()->GetWielded()->MoveTo(GetStack());
    }
  }

  if(GetMainWielded() != DigItem)
  {
    Dig->SetMoveDigger(true);
    DigItem->RemoveFromSlot();

    if(GetMainArm() && GetMainArm()->IsUsable())
      GetMainArm()->SetWielded(DigItem);
    else
      GetSecondaryArm()->SetWielded(DigItem);
  }
  else
    Dig->SetMoveDigger(false);

  Dig->SetSquareDug(Square);
  SetAction(Dig);
}

truth humanoid::CheckKick() const
{
  if(!CanKick())
  {
    if(IsPlayer())
      ADD_MESSAGE("This race can't kick.");

    return false;
  }

  if(GetUsableLegs() < 2)
  {
    if(IsPlayer())
      ADD_MESSAGE("How are you going to do that with %s?",
                  GetUsableLegs() ? "only one usable leg" : "no usable legs");

    return false;
  }
  else
    return true;
}

int humanoid::GetUsableLegs() const
{
  int Legs = 0;

  if(RightLegIsUsable())
    ++Legs;

  if(LeftLegIsUsable())
    ++Legs;

  return Legs;
}

int humanoid::GetUsableArms() const
{
  int Arms = 0;

  if(RightArmIsUsable())
    ++Arms;

  if(LeftArmIsUsable())
    ++Arms;

  return Arms;
}

truth humanoid::CheckThrow() const
{
  if(!character::CheckThrow())
    return false;

  if(HasAUsableArm())
    return true;
  else
  {
    ADD_MESSAGE("You don't have a usable arm to do that!");
    return false;
  }
}

truth humanoid::CheckOffer() const
{
  if(HasAUsableArm())
    return true;
  else
  {
    ADD_MESSAGE("You need a usable arm to offer.");
    return false;
  }
}

v2 humanoid::GetEquipmentPanelPos(int I) const // convert to array
{
  switch(I)
  {
   case HELMET_INDEX: return v2(34, -22);
   case AMULET_INDEX: return v2(14, -22);
   case CLOAK_INDEX: return v2(-6, -22);
   case BODY_ARMOR_INDEX: return v2(54, -22);
   case BELT_INDEX: return v2(24, 70);
   case RIGHT_WIELDED_INDEX: return v2(-14, 4);
   case LEFT_WIELDED_INDEX: return v2(62, 4);
   case RIGHT_RING_INDEX: return v2(-14, 44);
   case LEFT_RING_INDEX: return v2(62, 44);
   case RIGHT_GAUNTLET_INDEX: return v2(-14, 24);
   case LEFT_GAUNTLET_INDEX: return v2(62, 24);
   case RIGHT_BOOT_INDEX: return v2(4, 70);
   case LEFT_BOOT_INDEX: return v2(44, 70);
  }

  return v2(24, 12);
}

v2 humanoid::SilhouetteWhere=v2(0,0); //zeroed because wont init properly here.. TODO explain why.
v2 humanoid::SilhouetteWhereDefault=v2(0,0); //zeroed because wont init properly here.. TODO explain why.
void humanoid::DrawSilhouette(truth AnimationDraw) const
{
  int c;
  blitdata B1 = { DOUBLE_BUFFER,
                  { 0, 0 },
                  { 0, 0 },
                  { TILE_SIZE, TILE_SIZE },
                  { ivanconfig::GetContrastLuminance() },
                  TRANSPARENT_COLOR,
                  ALLOW_ANIMATE };

  cint Equipments = GetEquipments();

  if(SilhouetteWhereDefault.Is0())SilhouetteWhereDefault={RES.X - SILHOUETTE_SIZE.X - 39, 53};
  if(SilhouetteWhere.Is0())SilhouetteWhere=SilhouetteWhereDefault;

  item* eqMHover = NULL;
  if(CanUseEquipment())
    for(c = 0; c < Equipments; ++c)
      if(GetBodyPartOfEquipment(c) && EquipmentIsAllowed(c))
      {
        v2 Pos = SilhouetteWhereDefault + GetEquipmentPanelPos(c);

        item* Equipment = GetEquipment(c);

        if(Equipment && globalwindowhandler::IsMouseAtRect(Pos,TILE_V2)){
          eqMHover = Equipment;
          static item* eqMHoverPrevious = NULL;
          if(eqMHoverPrevious != eqMHover){ //prevent spam
            festring fs;
            Equipment->AddInventoryEntry(PLAYER,fs,1,true); //to show AV DAM weight volume
            ADD_MESSAGE("Your %s is %s.",GetEquipmentName(c),fs.CStr());
            msgsystem::Draw();
            eqMHoverPrevious = eqMHover;
          }
        }

        if(!AnimationDraw || eqMHover!=NULL)
          DOUBLE_BUFFER->DrawRectangle(Pos + v2(-1, -1), Pos + TILE_V2, DARK_GRAY);
//            eqMHover==Equipment ? LIGHT_GRAY : DARK_GRAY);

        if(Equipment && (!AnimationDraw || Equipment->IsAnimated() || eqMHover!=NULL))
        {
          igraph::BlitBackGround(Pos, TILE_V2);
          B1.Dest = Pos;

          if(Equipment->AllowAlphaEverywhere())
            B1.CustomData |= ALLOW_ALPHA;

          Equipment->Draw(B1);
          B1.CustomData &= ~ALLOW_ALPHA;

//          if(eqMHover==Equipment){
//            v2 v2M = globalwindowhandler::GetMouseLocation();
//            if(ivanconfig::GetSilhouetteScale()>=2)
//              DOUBLE_BUFFER->DrawLine(v2M,v2M+v2(1,1),WHITE,true); //mouse "dot"
//          }
        }
      }

  if(!AnimationDraw)
  {
    blitdata B2 = { DOUBLE_BUFFER,
                    { 0, 0 },
                    { SilhouetteWhere.X + 8, SilhouetteWhere.Y },
                    { SILHOUETTE_SIZE.X, SILHOUETTE_SIZE.Y },
                    { 0 },
                    0,
                    0 };

    for(int c = 0; c < BodyParts; ++c)
    {
      bodypart* BodyPart = GetBodyPart(c);

      if(BodyPart)
      {
        int Type = BodyPart->IsUsable() ? SILHOUETTE_NORMAL : SILHOUETTE_INTER_LACED;
        bitmap* Cache = igraph::GetSilhouetteCache(c, BodyPart->GetConditionColorIndex(), Type);
        Cache->NormalMaskedBlit(B2);
        BodyPart->DrawScars(B2);
      }
    }
  }
}

int humanoid::GetGlobalResistance(int Type) const
{
  int Resistance = GetResistance(Type);

  if(GetCloak())
    Resistance += GetCloak()->GetResistance(Type);

  if(GetRightWielded())
  {
    if(GetRightWielded()->IsShield(this))
      Resistance += GetRightWielded()->GetResistance(Type);
  }

  if(GetLeftWielded())
  {
    if(GetLeftWielded()->IsShield(this))
      Resistance += GetLeftWielded()->GetResistance(Type);
  }

  if(!(Type & PHYSICAL_DAMAGE))
  {
    if(GetAmulet())
      Resistance += GetAmulet()->GetResistance(Type);

    if(GetRightRing())
      Resistance += GetRightRing()->GetResistance(Type);

    if(GetLeftRing())
      Resistance += GetLeftRing()->GetResistance(Type);
  }

  return Resistance;
}

truth humanoid::TryToRiseFromTheDead()
{
  int c;

  for(c = 0; c < BodyParts; ++c)
    if(!GetBodyPart(c))
    {
      bodypart* BodyPart = SearchForOriginalBodyPart(c);

      if(BodyPart)
      {
        BodyPart->RemoveFromSlot();
        AttachBodyPart(BodyPart);
        BodyPart->SetHP(1);
      }
    }

  for(c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(BodyPartIsVital(c) && !BodyPart)
      if(!HandleNoBodyPart(c))
        return false;

    if(BodyPart)
    {
      if(BodyPart->IsBurning())
      {
        BodyPart->Extinguish(false);
        BodyPart->ResetBurning();
        BodyPart->ResetThermalEnergies();
        BodyPart->ResetSpoiling();
        BodyPart->SignalEmitationDecrease(MakeRGB24(150, 120, 90)); // gum solution
      }

      if(BodyPart->CanRegenerate() || BodyPart->GetHP() < 1)
        BodyPart->SetHP(1);
    }
  }

  ResetStates();
  return true;
}

truth humanoid::HandleNoBodyPart(int I)
{
  switch(I)
  {
   case HEAD_INDEX:
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("The headless body of %s vibrates violently.", CHAR_NAME(DEFINITE));

    return false;
   case GROIN_INDEX:
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("The groinless body of %s vibrates violently.", CHAR_NAME(DEFINITE));

    return false;
   case TORSO_INDEX:
    ABORT("The corpse does not have a torso.");
   default:
    return true;
  }
}

v2 humanoid::GetBodyPartBitmapPos(int I, truth) const
{
  switch(I)
  {
   case TORSO_INDEX: return GetTorsoBitmapPos();
   case HEAD_INDEX: return GetHeadBitmapPos();
   case RIGHT_ARM_INDEX: return GetRightArmBitmapPos();
   case LEFT_ARM_INDEX: return GetLeftArmBitmapPos();
   case GROIN_INDEX: return GetGroinBitmapPos();
   case RIGHT_LEG_INDEX: return GetRightLegBitmapPos();
   case LEFT_LEG_INDEX: return GetLeftLegBitmapPos();
  }

  ABORT("Weird bodypart BitmapPos request for a humanoid!");
  return v2();
}

col16 humanoid::GetBodyPartColorB(int I, truth) const
{
  switch(I)
  {
   case TORSO_INDEX: return GetTorsoMainColor();
   case HEAD_INDEX: return GetCapColor();
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX: return GetArmMainColor();
   case GROIN_INDEX:
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX: return GetLegMainColor();
  }

  ABORT("Weird bodypart col B request for a humanoid!");
  return 0;
}

col16 humanoid::GetBodyPartColorC(int I, truth) const
{
  switch(I)
  {
   case TORSO_INDEX: return GetBeltColor();
   case HEAD_INDEX: return GetHairColor();
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX: return GetGauntletColor();
   case GROIN_INDEX:
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX: return GetBootColor();
  }

  ABORT("Weird bodypart col C request for a humanoid!");
  return 0;
}

col16 humanoid::GetBodyPartColorD(int I, truth) const
{
  switch(I)
  {
   case TORSO_INDEX: return GetTorsoSpecialColor();
   case HEAD_INDEX: return GetEyeColor();
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX: return GetArmSpecialColor();
   case GROIN_INDEX:
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX: return GetLegSpecialColor();
  }

  ABORT("Weird bodypart col D request for a humanoid!");
  return 0;
}

int humanoid::GetBodyPartSparkleFlags(int I) const
{
  truth Sparkling = false;
  int SparkleFlags = GetNaturalSparkleFlags() & SKIN_COLOR ? SPARKLING_A : 0;

  switch(I)
  {
   case TORSO_INDEX: Sparkling = GetNaturalSparkleFlags() & TORSO_MAIN_COLOR; break;
   case HEAD_INDEX: Sparkling = GetNaturalSparkleFlags() & CAP_COLOR; break;
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX: Sparkling = GetNaturalSparkleFlags() & ARM_MAIN_COLOR; break;
   case GROIN_INDEX:
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX: Sparkling = GetNaturalSparkleFlags() & LEG_MAIN_COLOR; break;
  }

  SparkleFlags |= Sparkling ? SPARKLING_B : 0;
  Sparkling = false;

  switch(I)
  {
   case TORSO_INDEX: Sparkling = GetNaturalSparkleFlags() & BELT_COLOR; break;
   case HEAD_INDEX: Sparkling = GetNaturalSparkleFlags() & HAIR_COLOR; break;
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX: Sparkling = GetNaturalSparkleFlags() & GAUNTLET_COLOR; break;
   case GROIN_INDEX:
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX: Sparkling = GetNaturalSparkleFlags() & BOOT_COLOR; break;
  }

  SparkleFlags |= Sparkling ? SPARKLING_C : 0;
  Sparkling = false;

  switch(I)
  {
   case TORSO_INDEX: Sparkling = GetNaturalSparkleFlags() & TORSO_SPECIAL_COLOR; break;
   case HEAD_INDEX: Sparkling = GetNaturalSparkleFlags() & EYE_COLOR; break;
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX: Sparkling = GetNaturalSparkleFlags() & ARM_SPECIAL_COLOR; break;
   case GROIN_INDEX:
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX: Sparkling = GetNaturalSparkleFlags() & LEG_SPECIAL_COLOR; break;
  }

  SparkleFlags |= Sparkling ? SPARKLING_D : 0;
  return SparkleFlags;
}

playerkind::playerkind() : SoulID(0), IsBonePlayer(false), IsClone(false)
{
}

petrus::petrus() : LastHealed(0)
{
  game::SetPetrus(this);
}

shopkeeper::shopkeeper()
{
  if(!game::IsLoading())
    Money = GetMoney() + RAND() % 2001;
}

void humanoid::Bite(character* Enemy, v2 HitPos, int Direction, truth ForceHit)
{
  /* This function ought not to be called without a head */

  EditNP(-50);
  EditAP(-GetHead()->GetBiteAPCost());
  EditExperience(AGILITY, 150, 1 << 9);
  EditStamina(GetAdjustedStaminaCost(-1000, GetAttribute(AGILITY)), false);
  Enemy->TakeHit(this, 0, GetHead(), HitPos, GetHead()->GetBiteDamage(), GetHead()->GetBiteToHitValue(),
                 RAND() % 26 - RAND() % 26, BITE_ATTACK, Direction, !(RAND() % GetCriticalModifier()), ForceHit);
}

void humanoid::Kick(lsquare* Square, int Direction, truth ForceHit)
{
  leg* KickLeg = RAND_2 ? GetRightLeg() : GetLeftLeg();
  item* Boot = KickLeg->GetBoot();
  EditNP(-50);
  EditAP(-KickLeg->GetKickAPCost());
  EditStamina(GetAdjustedStaminaCost(-1000, GetAttribute(LEG_STRENGTH)), false);

  if(Square->BeKicked(this, Boot, KickLeg, KickLeg->GetKickDamage(), KickLeg->GetKickToHitValue(),
                      RAND() % 26 - RAND() % 26, Direction, !(RAND() % GetCriticalModifier()), ForceHit))
  {
    KickLeg->EditExperience(LEG_STRENGTH, 75, 1 << 9);
    KickLeg->EditExperience(AGILITY, 75, 1 << 9);
  }
}

/* Returns the average number of APs required to kill Enemy */

double humanoid::GetTimeToKill(ccharacter* Enemy, truth UseMaxHP) const
{
  double Effectivity = 0;
  int AttackStyles = 0;

  if(IsUsingArms())
  {
    arm* RightArm = GetRightArm();

    if(RightArm)
    {
      double Damage = RightArm->GetDamage();

      if(Damage)
        Effectivity += 1 / (Enemy->GetTimeToDie(this, int(Damage) + 1, RightArm->GetToHitValue(),
                                                AttackIsBlockable(GetRightWielded() ? WEAPON_ATTACK : UNARMED_ATTACK),
                                                UseMaxHP) * RightArm->GetAPCost());
    }

    arm* LeftArm = GetLeftArm();

    if(LeftArm)
    {
      double Damage = LeftArm->GetDamage();

      if(Damage)
        Effectivity += 1 / (Enemy->GetTimeToDie(this, int(Damage) + 1, LeftArm->GetToHitValue(),
                                                AttackIsBlockable(GetLeftWielded() ? WEAPON_ATTACK : UNARMED_ATTACK),
                                                UseMaxHP) * LeftArm->GetAPCost());
    }

    ++AttackStyles;
  }

  if(IsUsingLegs())
  {
    leg* RightLeg = GetRightLeg();
    leg* LeftLeg = GetLeftLeg();
    double TimeToDie = Enemy->GetTimeToDie(this, int(RightLeg->GetKickDamage()) + 1, RightLeg->GetKickToHitValue(),
                                           AttackIsBlockable(KICK_ATTACK), UseMaxHP) * RightLeg->GetKickAPCost()
                       + Enemy->GetTimeToDie(this, int(LeftLeg->GetKickDamage()) + 1, LeftLeg->GetKickToHitValue(),
                                             AttackIsBlockable(KICK_ATTACK), UseMaxHP) * LeftLeg->GetKickAPCost();
    Effectivity += 2 / TimeToDie;
    ++AttackStyles;
  }

  if(IsUsingHead())
  {
    head* Head = GetHead();
    Effectivity += 1 / (Enemy->GetTimeToDie(this, int(Head->GetBiteDamage()) + 1, Head->GetBiteToHitValue(),
                                            AttackIsBlockable(BITE_ATTACK), UseMaxHP) * Head->GetBiteAPCost());
    ++AttackStyles;
  }

  if(StateIsActivated(HASTE))
    Effectivity *= 2;

  if(StateIsActivated(SLOW))
    Effectivity /= 2;

  return AttackStyles ? AttackStyles / Effectivity : 10000000;
}

int humanoid::GetAttribute(int Identifier, truth AllowBonus) const
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::GetAttribute(Identifier, AllowBonus);
  else
  {
    int Attrib = 0;

    if(Identifier == ARM_STRENGTH || Identifier == DEXTERITY)
    {
      arm* RightArm = GetRightArm();

      if(RightArm)
        Attrib += RightArm->GetAttribute(Identifier, AllowBonus);

      arm* LeftArm = GetLeftArm();

      if(LeftArm)
        Attrib += LeftArm->GetAttribute(Identifier, AllowBonus);
    }
    else if(Identifier == LEG_STRENGTH || Identifier == AGILITY)
    {
      leg* RightLeg = GetRightLeg();

      if(RightLeg)
        Attrib += RightLeg->GetAttribute(Identifier, AllowBonus);

      leg* LeftLeg = GetLeftLeg();

      if(LeftLeg)
        Attrib += LeftLeg->GetAttribute(Identifier, AllowBonus);
    }
    else
    {
      ABORT("Illegal humanoid attribute %d request!", Identifier);
      return 0xEBBA;
    }

    return Attrib >> 1;
  }
}

truth humanoid::EditAttribute(int Identifier, int Value)
{
  if(Identifier < BASE_ATTRIBUTES)
    return character::EditAttribute(Identifier, Value);
  else if(Identifier == ARM_STRENGTH || Identifier == DEXTERITY)
  {
    truth Success = false;

    if(GetRightArm() && GetRightArm()->EditAttribute(Identifier, Value))
      Success = true;

    if(GetLeftArm() && GetLeftArm()->EditAttribute(Identifier, Value))
      Success = true;

    return Success;
  }
  else if(Identifier == LEG_STRENGTH || Identifier == AGILITY)
  {
    truth Success = false;

    if(GetRightLeg() && GetRightLeg()->EditAttribute(Identifier, Value))
      Success = true;

    if(GetLeftLeg() && GetLeftLeg()->EditAttribute(Identifier, Value))
      Success = true;

    return Success;
  }
  else
  {
    ABORT("Illegal humanoid attribute %d edit request!", Identifier);
    return false;
  }
}

void humanoid::EditExperience(int Identifier, double Value, double Speed)
{
  if(!AllowExperience())
    return;

  if(Identifier < BASE_ATTRIBUTES)
    character::EditExperience(Identifier, Value, Speed);
  else if(Identifier == ARM_STRENGTH || Identifier == DEXTERITY)
  {
    if(GetRightArm())
      GetRightArm()->EditExperience(Identifier, Value, Speed);

    if(GetLeftArm())
      GetLeftArm()->EditExperience(Identifier, Value, Speed);
  }
  else if(Identifier == LEG_STRENGTH || Identifier == AGILITY)
  {
    if(GetRightLeg())
      GetRightLeg()->EditExperience(Identifier, Value, Speed);

    if(GetLeftLeg())
      GetLeftLeg()->EditExperience(Identifier, Value, Speed);
  }
  else
    ABORT("Illegal humanoid attribute %d experience edit request!", Identifier);
}

int humanoid::DrawStats(truth AnimationDraw) const
{
  DrawSilhouette(AnimationDraw);

  if(AnimationDraw)
    return 15;

  int PanelPosX = RES.X - 96, PanelPosY = 15;
  PrintAttribute("AStr", ARM_STRENGTH, PanelPosX, PanelPosY++);
  PrintAttribute("LStr", LEG_STRENGTH, PanelPosX, PanelPosY++);
  PrintAttribute("Dex", DEXTERITY, PanelPosX, PanelPosY++);
  PrintAttribute("Agi", AGILITY, PanelPosX, PanelPosY++);
  return PanelPosY;
}

int humanoid::GetRandomStepperBodyPart() const
{
  int Possible = 0, PossibleArray[3];

  if(GetRightLeg())
    PossibleArray[Possible++] = RIGHT_LEG_INDEX;

  if(GetLeftLeg())
    PossibleArray[Possible++] = LEFT_LEG_INDEX;

  if(Possible)
    return PossibleArray[RAND_N(Possible)];

  if(GetRightArm())
    PossibleArray[Possible++] = RIGHT_ARM_INDEX;

  if(GetLeftArm())
    PossibleArray[Possible++] = LEFT_ARM_INDEX;

  if(Possible)
    return PossibleArray[RAND_N(Possible)];

  if(GetHead())
    PossibleArray[Possible++] = HEAD_INDEX;

  if(GetGroin())
    PossibleArray[Possible++] = GROIN_INDEX;

  PossibleArray[Possible++] = TORSO_INDEX;
  return PossibleArray[RAND_N(Possible)];
}

int humanoid::CheckForBlock(character* Enemy, item* Weapon, double ToHitValue, int Damage, int Success, int Type)
{
  if(GetAction())
    return Damage;

  if(GetRightWielded())
    Damage = CheckForBlockWithArm(Enemy, Weapon, GetRightArm(), ToHitValue, Damage, Success, Type);

  if(Damage && GetLeftWielded() && (!Weapon || Weapon->Exists()))
    Damage = CheckForBlockWithArm(Enemy, Weapon, GetLeftArm(), ToHitValue, Damage, Success, Type);

  return Damage;
}

truth humanoid::CanWield() const
{
  return CanUseEquipment(RIGHT_WIELDED_INDEX) || CanUseEquipment(LEFT_WIELDED_INDEX);
}

/* return true if still in balance */

truth humanoid::CheckBalance(double KickDamage)
{
  return !CanMove()
    || IsStuck()
    || !KickDamage
    || (GetUsableLegs() != 1
        && !IsFlying()
        && KickDamage * 5 < RAND() % GetSize());
}

long humanoid::GetMoveAPRequirement(int Difficulty) const
{
  if(IsFlying())
    return (!StateIsActivated(PANIC) ? 10000000 : 8000000) * Difficulty
           / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());

  switch(GetUsableLegs())
  {
   case 0:
    return (!StateIsActivated(PANIC) ? 20000000 : 16000000) * Difficulty
           / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());
   case 1:
    return (!StateIsActivated(PANIC) ? 13333333 : 10666667) * Difficulty
           / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());
   case 2:
    return (!StateIsActivated(PANIC) ? 10000000 : 8000000) * Difficulty
           / (APBonus(GetAttribute(AGILITY)) * GetMoveEase());
  }

  ABORT("A %d legged humanoid invaded the dungeon!", GetUsableLegs());
  return 0;
}

void hunter::CreateBodyParts(int SpecialFlags)
{
  for(int c = 0; c < BodyParts; ++c)
    if(c != LEFT_ARM_INDEX)
      CreateBodyPart(c, SpecialFlags);
    else
      SetBodyPart(LEFT_ARM_INDEX, 0);
}

truth humanoid::EquipmentEasilyRecognized(int I) const
{
  if(GetRelation(PLAYER) != HOSTILE)
    return true;

  switch(I)
  {
   case AMULET_INDEX:
   case RIGHT_RING_INDEX:
   case LEFT_RING_INDEX:
   case BELT_INDEX:
    return false;
  }

  return true;
}

void humanoid::SignalEquipmentAdd(int EquipmentIndex)
{
  character::SignalEquipmentAdd(EquipmentIndex);

  if(EquipmentIndex == RIGHT_WIELDED_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentRightSWeaponSkill, GetRightWielded());
  else if(EquipmentIndex == LEFT_WIELDED_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentLeftSWeaponSkill, GetLeftWielded());

  if(!IsInitializing())
    CalculateBattleInfo();
}

void humanoid::SignalEquipmentRemoval(int EquipmentIndex, citem* Item)
{
  character::SignalEquipmentRemoval(EquipmentIndex, Item);

  if(EquipmentIndex == RIGHT_WIELDED_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentRightSWeaponSkill, 0);
  else if(EquipmentIndex == LEFT_WIELDED_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentLeftSWeaponSkill, 0);

  if(!IsInitializing())
    CalculateBattleInfo();
}

void humanoid::SWeaponSkillTick()
{
  for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end();)
  {
    if((*i)->Tick() && IsPlayer())
    {
      item* Item = SearchForItem(*i);

      if(Item)
        (*i)->AddLevelDownMessage(Item->CHAR_NAME(UNARTICLED));
    }

    if(!(*i)->GetHits() && *i != GetCurrentRightSWeaponSkill() && *i != GetCurrentLeftSWeaponSkill())
    {
      std::list<sweaponskill*>::iterator Dirt = i++;
      SWeaponSkill.erase(Dirt);
    }
    else
      ++i;
  }
}

void angel::GetAICommand()
{
  if((LastHealed || game::GetTick() - LastHealed > 10000) && AttachBodyPartsOfFriendsNear())
    return;

  humanoid::GetAICommand();
}

/* Returns true if the angel finds somebody near to heal else false */

truth angel::AttachBodyPartsOfFriendsNear()
{
  character* HurtOne = 0;
  bodypart* SeveredOne = 0;

  for(int d = 0; d < GetNeighbourSquares(); ++d)
  {
    square* Square = GetNeighbourSquare(d);

    if(Square)
    {
      character* Char = Square->GetCharacter();

      if(Char && (!HurtOne || Char->IsPlayer()) && GetRelation(Char) == FRIEND && !Char->HasAllBodyParts())
      {
        bodypart* BodyPart = Char->FindRandomOwnBodyPart(false);

        if(BodyPart)
        {
          HurtOne = Char;
          SeveredOne = BodyPart;
        }
      }
    }
  }

  if(HurtOne)
  {
    if(HurtOne->IsPlayer())
      ADD_MESSAGE("%s puts your %s back to its place.",
                  CHAR_DESCRIPTION(DEFINITE), SeveredOne->GetBodyPartName().CStr());
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s helps %s by putting %s %s in its old place.",
                  CHAR_DESCRIPTION(DEFINITE), HurtOne->CHAR_DESCRIPTION(DEFINITE),
                  HurtOne->GetPossessivePronoun().CStr(), SeveredOne->GetBodyPartName().CStr());

    SeveredOne->SetHP(1);
    SeveredOne->RemoveFromSlot();
    HurtOne->AttachBodyPart(SeveredOne);
    LastHealed = game::GetTick();
    DexterityAction(10);
    return true;
  }
  else
    return false;
}

void humanoid::DrawBodyParts(blitdata& BlitData) const
{
  bitmap* TileBuffer = igraph::GetTileBuffer();
  bitmap* RealBitmap = BlitData.Bitmap;
  blitdata B = { TileBuffer,
                 { BlitData.Dest.X, BlitData.Dest.Y },
                 { 0, 0 },
                 { TILE_SIZE, TILE_SIZE },
                 { 0 },
                 TRANSPARENT_COLOR,
                 BlitData.CustomData };

  RealBitmap->NormalBlit(B);
  TileBuffer->FillPriority(0);
  B.Src.X = B.Src.Y = 0;
  B.Luminance = BlitData.Luminance;

  for(int c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(DrawOrder[c]);

    if(BodyPart)
      BodyPart->Draw(B);
  }

  arm* LeftArm = GetLeftArm();

  if(LeftArm)
    LeftArm->DrawWielded(B);

  arm* RightArm = GetRightArm();

  if(RightArm)
    RightArm->DrawWielded(B);

  TileBuffer->FastBlit(RealBitmap, BlitData.Dest);
}

col16 angel::GetTorsoMainColor() const
{
  return GetMasterGod()->GetColor();
}

col16 angel::GetArmMainColor() const
{
  return GetMasterGod()->GetColor();
}

col16 kamikazedwarf::GetTorsoMainColor() const
{
  return GetMasterGod()->GetColor();
}

col16 kamikazedwarf::GetGauntletColor() const
{
  return GetMasterGod()->GetColor();
}

col16 kamikazedwarf::GetLegMainColor() const
{
  return GetMasterGod()->GetColor();
}

col16 housewife::GetHairColor() const
{
  static col16 HouseWifeHairColor[] = { MakeRGB16(48, 40, 8), MakeRGB16(60, 48, 24), MakeRGB16(200, 0, 0) };
  return HouseWifeHairColor[RAND() % 3];
}

int angel::GetAttribute(int Identifier, truth AllowBonus) const // temporary until wings are bodyparts
{
  if(Identifier == LEG_STRENGTH)
    return GetDefaultLegStrength();
  else if(Identifier == AGILITY)
    return GetDefaultAgility();
  else
    return humanoid::GetAttribute(Identifier, AllowBonus);
}

int nihil::GetAttribute(int Identifier, truth AllowBonus) const
{
  if(Identifier == LEG_STRENGTH)
    return GetDefaultLegStrength();
  else if(Identifier == AGILITY)
    return GetDefaultAgility();
  else
    return humanoid::GetAttribute(Identifier, AllowBonus);
}

int genie::GetAttribute(int Identifier, truth AllowBonus) const // temporary until someone invents a better way of doing this
{
  if(Identifier == LEG_STRENGTH)
    return GetDefaultLegStrength();
  else if(Identifier == AGILITY)
    return GetDefaultAgility();
  else
    return humanoid::GetAttribute(Identifier, AllowBonus);
}

truth humanoid::CanUseStethoscope(truth PrintReason) const
{
  if(!GetUsableArms())
  {
    if(PrintReason)
      ADD_MESSAGE("You need a usable arm to use a stethoscope.");

    return false;
  }

  if(!GetHead())
  {
    if(PrintReason)
      ADD_MESSAGE("You need a head to use stethoscope.");

    return false;
  }

  return true;
}

truth humanoid::IsUsingArms() const
{
  return GetAttackStyle() & USE_ARMS && CanAttackWithAnArm();
}

truth humanoid::IsUsingLegs() const
{
  return (GetAttackStyle() & USE_LEGS
          || (GetAttackStyle() & USE_ARMS && !CanAttackWithAnArm()))
    && HasTwoUsableLegs();
}

truth humanoid::IsUsingHead() const
{
  return (GetAttackStyle() & USE_HEAD
          || ((GetAttackStyle() & USE_LEGS
               || (GetAttackStyle() & USE_ARMS && !CanAttackWithAnArm()))
              && !HasTwoUsableLegs()))
    && GetHead();
}

void humanoid::CalculateBattleInfo()
{
  CalculateDodgeValue();
  doforbodyparts()(this, &bodypart::CalculateAttackInfo);
}

item* skeleton::SevereBodyPart(int BodyPartIndex, truth ForceDisappearance, stack* EquipmentDropStack)
{
  if(BodyPartIndex == RIGHT_ARM_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentRightSWeaponSkill, 0);
  else if(BodyPartIndex == LEFT_ARM_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentLeftSWeaponSkill, 0);

  item* BodyPart = GetBodyPart(BodyPartIndex);
  item* Bone = 0;

  if(!ForceDisappearance)
  {
    if(BodyPartIndex == HEAD_INDEX)
    {
      if(GetConfig() == WAR_LORD)
        Bone = skullofxinroch::Spawn(0, NO_MATERIALS);
      else
        Bone = skull::Spawn(0, NO_MATERIALS);
    }
    else
      Bone = bone::Spawn(0, NO_MATERIALS);

    material* OldMaterial = BodyPart->GetMainMaterial();
    Bone->InitMaterials(OldMaterial);
    BodyPart->DropEquipment(EquipmentDropStack);
    BodyPart->RemoveFromSlot();
    BodyPart->SetMainMaterial(0, NO_PIC_UPDATE|NO_SIGNALS);
  }
  else
  {
    BodyPart->DropEquipment(EquipmentDropStack);
    BodyPart->RemoveFromSlot();
  }

  BodyPart->SendToHell();
  CalculateAttributeBonuses();
  CalculateBattleInfo();
  SignalPossibleTransparencyChange();
  RemoveTraps(BodyPartIndex);
  return Bone;
}

void zombie::CreateBodyParts(int SpecialFlags)
{
  bool Anyway = false;
  if((GetConfig() == ZOMBIE_OF_KHAZ_ZADM) || !!(SpecialFlags & NO_SEVERED_LIMBS))
  {
    Anyway = true;
  } // Khaz-Zadm needs her hands...

  for(int c = 0; c < BodyParts; ++c)
    if(Anyway || BodyPartIsVital(c) || RAND_N(3) || (c == HEAD_INDEX && !RAND_N(3)))
    {
      bodypart* BodyPart = CreateBodyPart(c, SpecialFlags|NO_PIC_UPDATE);
      BodyPart->GetMainMaterial()->SetSpoilCounter(2000 + RAND_N(1000));
    }
}

void ghost::AddName(festring& String, int Case) const
{
  if(OwnerSoul.IsEmpty() || Case & PLURAL)
    character::AddName(String, Case);
  else
  {
    character::AddName(String, (Case|ARTICLE_BIT)&~INDEFINE_BIT);
    String << " of " << OwnerSoul;
  }
}

void ghost::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << OwnerSoul << Active << Description;
}

void ghost::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> OwnerSoul >> Active >> Description;
}

void bonesghost::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << OwnerSoul << Active << Description << EyeColor << HairColor;
}

void bonesghost::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> OwnerSoul >> Active >> Description >> EyeColor >> HairColor;
}

truth ghost::RaiseTheDead(character* Summoner)
{
  itemvector ItemVector;
  GetStackUnder()->FillItemVector(ItemVector);

  for(uint c = 0; c < ItemVector.size(); ++c)
    if(ItemVector[c]->SuckSoul(this, Summoner))
      return true;

  if(IsPlayer())
    ADD_MESSAGE("You shudder.");
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s shudders.", CHAR_NAME(DEFINITE));

  return false;
}

int ghost::ReceiveBodyPartDamage(character* Damager, int Damage, int Type, int BodyPartIndex,
                                 int Direction, truth PenetrateResistance, truth Critical,
                                 truth ShowNoDamageMsg, truth CaptureBodyPart)
{
  if(Type != SOUND)
  {
    Active = true;
    return character::ReceiveBodyPartDamage(Damager, Damage, Type, BodyPartIndex, Direction,
                                            PenetrateResistance, Critical, ShowNoDamageMsg, CaptureBodyPart);
  }
  else
    return 0;
}

void ghost::GetAICommand()
{
  if(Active)
    character::GetAICommand();
  else
  {
    if(CheckForEnemies(false, false, false))
      return;

    EditAP(-1000);
  }
}

void humanoid::AddSpecialEquipmentInfo(festring& String, int I) const
{
  if((I == RIGHT_WIELDED_INDEX && GetRightArm()->TwoHandWieldIsActive()) ||
     (I == LEFT_WIELDED_INDEX && GetLeftArm()->TwoHandWieldIsActive()))
    String << " (in both hands)";
}

/* Yes, this is evil. */

#define INSTANTIATE(name)\
if(DataBase->name.IsValid() && (Item = DataBase->name.Instantiate(SpecialFlags)))\
  Set##name(Item);

void humanoid::CreateInitialEquipment(int SpecialFlags)
{
  character::CreateInitialEquipment(SpecialFlags);
  item* Item;

  INSTANTIATE(Helmet);
  INSTANTIATE(Amulet);
  INSTANTIATE(Cloak);
  INSTANTIATE(BodyArmor);
  INSTANTIATE(Belt);
  INSTANTIATE(RightWielded);
  INSTANTIATE(LeftWielded);
  INSTANTIATE(RightRing);
  INSTANTIATE(LeftRing);
  INSTANTIATE(RightGauntlet);
  INSTANTIATE(LeftGauntlet);
  INSTANTIATE(RightBoot);
  INSTANTIATE(LeftBoot);

  if(CurrentRightSWeaponSkill)
    CurrentRightSWeaponSkill->AddHit(GetRightSWeaponSkillHits() * 100);

  if(CurrentLeftSWeaponSkill)
    CurrentLeftSWeaponSkill->AddHit(GetLeftSWeaponSkillHits() * 100);
}

festring humanoid::GetBodyPartName(int I, truth Articled) const
{
  festring Article;

  if(Articled)
    Article << 'a';

  switch(I)
  {
   case HEAD_INDEX: return Article + "head";
   case TORSO_INDEX: return Article + "torso";
   case RIGHT_ARM_INDEX: return Article + "right arm";
   case LEFT_ARM_INDEX: return Article + "left arm";
   case GROIN_INDEX: return Article + "groin";
   case RIGHT_LEG_INDEX: return Article + "right leg";
   case LEFT_LEG_INDEX: return Article + "left leg";
  }

  ABORT("Illegal humanoid bodypart name request!");
  return "";
}

void humanoid::CreateBlockPossibilityVector(blockvector& Vector, double ToHitValue) const
{
  double RightBlockChance = 0;
  int RightBlockCapability = 0;
  double LeftBlockChance = 0;
  int LeftBlockCapability = 0;
  arm* RightArm = GetRightArm();
  arm* LeftArm = GetLeftArm();

  if(RightArm)
  {
    RightBlockChance = RightArm->GetBlockChance(ToHitValue);
    RightBlockCapability = RightArm->GetBlockCapability();
  }

  if(LeftArm)
  {
    LeftBlockChance = LeftArm->GetBlockChance(ToHitValue);
    LeftBlockCapability = LeftArm->GetBlockCapability();
  }

  /* Double block */

  if(RightBlockCapability + LeftBlockCapability)
    Vector.push_back(std::make_pair(RightBlockChance * LeftBlockChance, RightBlockCapability + LeftBlockCapability));

  /* Right block */

  if(RightBlockCapability)
    Vector.push_back(std::make_pair(RightBlockChance * (1 - LeftBlockChance), RightBlockCapability));

  /* Left block */

  if(LeftBlockCapability)
    Vector.push_back(std::make_pair(LeftBlockChance * (1 - RightBlockChance), LeftBlockCapability));
}

item* humanoid::SevereBodyPart(int BodyPartIndex, truth ForceDisappearance, stack* EquipmentDropStack)
{
  if(BodyPartIndex == RIGHT_ARM_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentRightSWeaponSkill, 0);
  else if(BodyPartIndex == LEFT_ARM_INDEX)
    EnsureCurrentSWeaponSkillIsCorrect(CurrentLeftSWeaponSkill, 0);

  return character::SevereBodyPart(BodyPartIndex, ForceDisappearance, EquipmentDropStack);
}

humanoid::humanoid(const humanoid& Humanoid)
: mybase(Humanoid), CurrentRightSWeaponSkill(0), CurrentLeftSWeaponSkill(0)
{
  SWeaponSkill.resize(Humanoid.SWeaponSkill.size());
  std::list<sweaponskill*>::const_iterator i2 = Humanoid.SWeaponSkill.begin();

  for(sweaponskill*& p : SWeaponSkill)
    p = new sweaponskill(**i2++);
}

cfestring& humanoid::GetDeathMessage() const
{
  static festring HeadlessDeathMsg = CONST_S("@Dd dies without a sound.");

  if(GetHead() || character::GetDeathMessage() != "@Dd dies screaming.")
    return character::GetDeathMessage();
  else
    return HeadlessDeathMsg;
}

int humanoid::GetSWeaponSkillLevel(citem* Item) const
{
  for(sweaponskill* p : SWeaponSkill)
    if(p->IsSkillOf(Item))
      return p->GetLevel();

  for(idholder* I = Item->GetCloneMotherID(); I; I = I->Next)
    for(sweaponskill* p : SWeaponSkill)
      if(p->IsSkillOfCloneMother(Item, I->ID))
        return p->GetLevel();

  return 0;
}

truth humanoid::UseMaterialAttributes() const
{
  return combinebodypartpredicates()(this, &bodypart::UseMaterialAttributes, 0);
}

col24 angel::GetBaseEmitation() const
{
  switch(GetMasterGod()->GetBasicAlignment())
  {
   case GOOD: return MakeRGB24(150, 150, 150);
   case NEUTRAL: return MakeRGB24(120, 120, 150);
   case EVIL: return MakeRGB24(150, 110, 110);
  }

  return 0;
}

void bananagrower::BeTalkedTo()
{
  if(!GetPos().IsAdjacent(PLAYER->GetPos()))
    return;

  static long Said;

  if(GetRelation(PLAYER) == HOSTILE)
    ProcessAndAddMessage(GetHostileReplies()[RandomizeReply(Said, GetHostileReplies().Size)]);
  else if(!game::TweraifIsFree())
  {
    if(GetRelation(PLAYER) != HOSTILE
       && Profession.Find("president", 0) != festring::NPos && !(RAND() % 7))
      ADD_MESSAGE("\"I'm glad Petrus spared my life even though I was the president.\"");

    ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, 6)]);
  }
  else
    ProcessAndAddMessage(GetFriendlyReplies()[6 + RandomizeReply(Said, 3)]);
}

void bananagrower::RandomizeProfession()
{
  switch(RAND_N(12))
  {
   case 0:
    Profession = CONST_S("the president of Tweraif");
    break;
   case 1:
    Profession = CONST_S("a diplomat");
    break;
   case 2:
    Profession = CONST_S("a teacher");
    break;
   case 3:
    Profession = CONST_S("a philosopher");
    break;
   case 4:
    Profession = CONST_S("a journalist");
    break;
   case 5:
    Profession = CONST_S("an alchemist");
    break;
   case 6:
    Profession = CONST_S("a renowned mathematician");
    break;
   case 7:
    Profession = CONST_S("a priest of Silva");
    break;
   case 8:
   case 9:
   case 10:
   case 11:
    Profession = CONST_S("a professor of ");
    AddRandomScienceName(Profession);
    break;
  }
}

void bananagrower::PostConstruct()
{
  Stamina = MaxStamina / 5;
  RandomizeProfession();
  HasDroppedBananas = FeedingSumo = false;
}

void bananagrower::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << Profession << HasDroppedBananas << FeedingSumo;
}

void bananagrower::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> Profession >> HasDroppedBananas >> FeedingSumo;
}

void smith::BeTalkedTo()
{
  if(!GetPos().IsAdjacent(PLAYER->GetPos()))
    return;

  if(GetRelation(PLAYER) == HOSTILE)
  {
    ADD_MESSAGE("\"You talkin' to me? You talkin' to me? You talkin' to me? Then who "
                "the hell else are you talkin' to? You talkin' to me? Well I'm the "
                "only one here. Who do you think you're talking to? Oh yeah? Huh? Ok.\"");
    return;
  }

  if(!GetMainWielded() || !GetMainWielded()->CanBeUsedBySmith())
  {
    ADD_MESSAGE("\"Sorry, I need an intact hammer to practise the art of smithing.\"");
    return;
  }

  if(PLAYER->PossessesItem(&item::IsFixableBySmith))
  {
    item* Item = PLAYER->SelectFromPossessions(CONST_S("\"What do you want me to fix?\""), &item::IsFixableBySmith);

    if(!Item)
      return;

    if(!(Item->GetMainMaterial()->GetCategoryFlags() & IS_METAL))
    {
      ADD_MESSAGE("\"I only fix items made of metal.\"");
      return;
    }

    /** update messages */

    long FixPrice = Item->GetFixPrice();

    if(PLAYER->GetMoney() < FixPrice)
    {
      ADD_MESSAGE("\"Getting that fixed costs you %ld gold pieces. Get the money and we'll talk.\"", FixPrice);
      return;
    }

    ADD_MESSAGE("\"I can fix your %s, but it'll cost you %ld gold pieces.\"", Item->CHAR_NAME(UNARTICLED), FixPrice);

    if(game::TruthQuestion(CONST_S("Do you accept this deal? [y/N]")))
    {
      Item->RemoveRust();
      Item->Fix();
      PLAYER->EditMoney(-FixPrice);
      ADD_MESSAGE("%s fixes %s in no time.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(DEFINITE));
    }
  }
  else
    ADD_MESSAGE("\"Come back when you have some weapons or armor I can fix.\"");
}

void humanoid::CalculateDodgeValue()
{
  DodgeValue = 0.05 * GetMoveEase() * GetAttribute(AGILITY) / sqrt(GetSize());

  if(IsFlying())
    DodgeValue *= 2;
  else
  {
    if(!HasAUsableLeg())
      DodgeValue *= 0.50;
    if(!HasTwoUsableLegs())
      DodgeValue *= 0.75;
  }

  if(DodgeValue < 1)
    DodgeValue = 1;
}

truth humanoid::CheckZap()
{
  if(!GetUsableArms())
  {
    ADD_MESSAGE("You need at least one usable arm to zap.");
    return false;
  }
  else
    return character::CheckZap();
}

void bananagrower::GetAICommand()
{
  if(game::TweraifIsFree() ||
     (GetDungeon()->GetIndex() != NEW_ATTNAM)
   ) // Behave normally outside of New Attnam.
  {
    humanoid::GetAICommand();
    return;
  }

  if(CheckForEnemies(false, false, true, true))
    return;

  if(!IsEnabled())
    return;

  cv2 BananaTarget = FeedingSumo ? SUMO_ROOM_POS + v2(1, 2) : v2(45, 45);

  if(GetPos() == BananaTarget)
  {
    itemvector ItemVector;
    GetStack()->FillItemVector(ItemVector);
    int BananasDropped = 0;
    uint c;

    for(c = 0; c < ItemVector.size(); ++c)
      if(ItemVector[c]->IsBanana())
      {
        ItemVector[c]->MoveTo(GetStackUnder());
        ++BananasDropped;
      }

    if(BananasDropped)
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s drops %s.", CHAR_NAME(DEFINITE), BananasDropped == 1 ? "a banana" : "some bananas");

      return;
    }

    ItemVector.clear();
    GetStackUnder()->FillItemVector(ItemVector);
    int PeelsPickedUp = 0;

    for(c = 0; c < ItemVector.size(); ++c)
      if(ItemVector[c]->IsBananaPeel())
      {
        ItemVector[c]->MoveTo(GetStack());
        ++PeelsPickedUp;
      }

    if(PeelsPickedUp)
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s picks up %s.", CHAR_NAME(DEFINITE),
                    PeelsPickedUp == 1 ? "a banana peel" : "some banana peels");

      return;
    }

    HasDroppedBananas = true;
  }

  if(!HasDroppedBananas)
  {
    SetGoingTo(BananaTarget);

    if(MoveTowardsTarget(true))
      return;
  }
  else if(GetPos().X == 54)
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s leaves the town to gather more bananas.", CHAR_NAME(DEFINITE));

    GetStack()->Clean();
    character* Sumo = game::GetSumo();
    FeedingSumo = Sumo && Sumo->GetNP() < (SATIATED_LEVEL + BLOATED_LEVEL) >> 1 && !(RAND() % 15);
    int Bananas = FeedingSumo ? 3 : 10;

    for(int c = 0; c < Bananas; ++c)
      GetStack()->AddItem(banana::Spawn());

    v2 Where = GetLevel()->GetNearestFreeSquare(this, v2(0, 45));

    if(Where == ERROR_V2)
      Where = GetLevel()->GetRandomSquare(this, NOT_IN_ROOM); // this is odd but at least it doesn't crash

    Move(Where, true);
    RandomizeProfession();
    RestoreBodyParts();
    RestoreHP();
    Stamina = MaxStamina / 5;
    ResetStates();
    TemporaryState = 0;

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s enters the town.", CHAR_NAME(INDEFINITE));

    HasDroppedBananas = false;
  }
  else
  {
    SetGoingTo(v2(54, 45));

    if(MoveTowardsTarget(true))
      return;
  }

  EditAP(-1000);
}

truth humanoid::CheckTalk()
{
  if(!character::CheckTalk())
    return false;

  if(!GetHead())
  {
    ADD_MESSAGE("You need a head to talk.");
    return false;
  }

  return true;
}

truth angel::CanCreateBodyPart(int I) const
{
  return I == TORSO_INDEX || I == HEAD_INDEX || I == RIGHT_ARM_INDEX || I == LEFT_ARM_INDEX;
}

truth nihil::CanCreateBodyPart(int I) const
{
  return I == TORSO_INDEX || I == HEAD_INDEX || I == RIGHT_ARM_INDEX || I == LEFT_ARM_INDEX;
}

truth genie::CanCreateBodyPart(int I) const
{
  return I == TORSO_INDEX || I == HEAD_INDEX || I == RIGHT_ARM_INDEX || I == LEFT_ARM_INDEX;
}

truth bananagrower::HandleCharacterBlockingTheWay(character* Char, v2 Pos, int Dir)
{
  return Char->GetPos() == v2(45, 45) && (Displace(Char, true) || Hit(Char, Pos, Dir));
}

festring& bananagrower::ProcessMessage(festring& Msg) const
{
  character::ProcessMessage(Msg);
  SEARCH_N_REPLACE(Msg, "@pd", GetProfession());
  SEARCH_N_REPLACE(Msg, "@Pd", GetProfession().CapitalizeCopy());
  return Msg;
}

void elder::CreateBodyParts(int SpecialFlags)
{
  for(int c = 0; c < BodyParts; ++c)
    if(c != LEFT_LEG_INDEX)
      CreateBodyPart(c, SpecialFlags);
    else
      SetBodyPart(LEFT_LEG_INDEX, 0);
}

/*void encourager::GetAICommand()
{
  if(CheckForEnemies(true, true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(CheckForDoors())
    return;

  if(game::GetTick() - LastHit > 200)
  {
    static int NotDiagonal[] = { 1, 3, 4, 6 };

    for(int d = 0; d < 4; ++d)
    {
      square* Square = GetNeighbourSquare(NotDiagonal[d]);

      if(Square)
      {
        character* Char = Square->GetCharacter();

        if(Char && Char->IsBananaGrower() && Hit(Char, Square->GetPos(), NotDiagonal[d], true))
        {
          LastHit = game::GetTick();
          TerminateGoingTo();
          return;
        }
      }
    }
  }

  if(MoveTowardsHomePos())
    return;

  EditAP(-1000);
}*/

/*void encourager::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << LastHit;
}

void encourager::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> LastHit;
}*/

long skeleton::GetBodyPartVolume(int I) const
{
  switch(I)
  {
   case HEAD_INDEX: return 600;
   case TORSO_INDEX: return (GetTotalVolume() - 600) * 13 / 30;
   case RIGHT_ARM_INDEX:
   case LEFT_ARM_INDEX: return (GetTotalVolume() - 600) / 10;
   case GROIN_INDEX: return (GetTotalVolume() - 600) / 10;
   case RIGHT_LEG_INDEX:
   case LEFT_LEG_INDEX: return ((GetTotalVolume() - 600) << 1) / 15;
  }

  ABORT("Illegal humanoid bodypart volume request!");
  return 0;
}

truth humanoid::AutoPlayAIequip()
{
  item* iL = GetEquipment(LEFT_WIELDED_INDEX);
  item* iR = GetEquipment(RIGHT_WIELDED_INDEX);

  //every X turns remove all equipments
  bool bTryWieldNow=false;
  static int iLastReEquipAllTurn=-1;
  if(game::GetTurn()>(iLastReEquipAllTurn+150)){ DBG2(game::GetTurn(),iLastReEquipAllTurn);
    iLastReEquipAllTurn=game::GetTurn();
    for(int i=0;i<MAX_EQUIPMENT_SLOTS;i++){
      item* eq = GetEquipment(i);
      if(eq){eq->MoveTo(GetStack());SetEquipment(i,NULL);} //eq is moved to end of stack!
      if(iL==eq)iL=NULL;
      if(iR==eq)iR=NULL;
    }
//        if(iL!=NULL){iL->MoveTo(GetStack());iL=NULL;SetEquipment(LEFT_WIELDED_INDEX ,NULL);DBGLN;}
//        if(iR!=NULL){iR->MoveTo(GetStack());iR=NULL;SetEquipment(RIGHT_WIELDED_INDEX,NULL);DBGLN;}
    bTryWieldNow=true;
  }

  //wield some weapon from the inventory as the NPC AI is not working for the player TODO why?
  //every X turns try to wield
  static int iLastTryToWieldTurn=-1;
  if(bTryWieldNow || game::GetTurn()>(iLastTryToWieldTurn+10)){ DBG2(game::GetTurn(),iLastTryToWieldTurn);
    iLastTryToWieldTurn=game::GetTurn();
    bool bDoneLR=false;
    bool bL2H = iL && iL->IsTwoHanded();
    bool bR2H = iR && iR->IsTwoHanded();

    //2handed
    static int iTryWieldWhat=0; iTryWieldWhat++; DBG1(iTryWieldWhat);
    if(iTryWieldWhat%2==0){ //will try 2handed first, alternating. If player has only 2handeds, the 1handeds will not be wielded and it will use punches, what is good too for tests.
      if( !bDoneLR &&
          iL==NULL && GetBodyPartOfEquipment(LEFT_WIELDED_INDEX )!=NULL &&
          iR==NULL && GetBodyPartOfEquipment(RIGHT_WIELDED_INDEX)!=NULL
      ){
        static itemvector vitEqW;vitEqW.clear();GetStack()->FillItemVector(vitEqW);
        for(uint c = 0; c < vitEqW.size(); ++c){
          if(vitEqW[c]->IsWeapon(this) && vitEqW[c]->IsTwoHanded()){  DBG1(vitEqW[c]->GetNameSingular().CStr());
            vitEqW[c]->RemoveFromSlot();
            SetEquipment(clock()%2==0 ? LEFT_WIELDED_INDEX : RIGHT_WIELDED_INDEX, vitEqW[c]); //DBG3("Wield",iEqIndex,vitEqW[c]->GetName(DEFINITE).CStr());
            bDoneLR=true;
            break;
          }
        }
      }
    }

    //dual 1handed (if not 2hd already)
    if(!bDoneLR){
      for(int i=0;i<2;i++){
        int iChk=-1;
        if(i==0)iChk=LEFT_WIELDED_INDEX;
        if(i==1)iChk=RIGHT_WIELDED_INDEX;

        if(
            !bDoneLR &&
            (
              (iChk==LEFT_WIELDED_INDEX  && iL==NULL && GetBodyPartOfEquipment(LEFT_WIELDED_INDEX ) && !bR2H)
              ||
              (iChk==RIGHT_WIELDED_INDEX && iR==NULL && GetBodyPartOfEquipment(RIGHT_WIELDED_INDEX) && !bL2H)
            )
        ){
          static itemvector vitEqW;vitEqW.clear();GetStack()->FillItemVector(vitEqW);
          for(uint c = 0; c < vitEqW.size(); ++c){
            if(
                (vitEqW[c]->IsWeapon(this) && !vitEqW[c]->IsTwoHanded())
                ||
                vitEqW[c]->IsShield(this)
            ){ DBG1(vitEqW[c]->GetNameSingular().CStr());
              vitEqW[c]->RemoveFromSlot();
              SetEquipment(iChk, vitEqW[c]);
              bDoneLR=true;
              break;
            }
          }
        }
      }
    }

  }

  //every X turns try to use stuff from inv
  static int iLastTryToUseInvTurn=-1;
  if(game::GetTurn()>(iLastTryToUseInvTurn+5)){ DBG2(game::GetTurn(),iLastTryToUseInvTurn);
    iLastTryToUseInvTurn=game::GetTurn();

    //////////////////////////////// consume food/drink
    { //TODO let this happen for non-human too?
      static itemvector vitEqW;vitEqW.clear();GetStack()->FillItemVector(vitEqW);DBGLN;
      for(uint c = 0; c < vitEqW.size(); ++c){DBGLN;
        if(clock()%3!=0 && GetHungerState() >= BLOATED)break;DBGLN; //randomly let it vomit and activate all related flows *eew* xD

        //if(TryToConsume(vitEqW[c]))
        material* ConsumeMaterial = vitEqW[c]->GetConsumeMaterial(this);
        if(
          ConsumeMaterial!=NULL &&
          vitEqW[c]->IsConsumable() &&
          ConsumeItem(vitEqW[c], vitEqW[c]->GetConsumeMaterial(this)->GetConsumeVerb())
        ){
          DBG2("AutoPlayConsumed",vitEqW[c]->GetNameSingular().CStr());
          return true;
        }DBGLN;
      }
    }

    //////////////////////////////// equip
    {DBGLN;
      static itemvector vitEqW;vitEqW.clear();GetStack()->FillItemVector(vitEqW);DBGLN;
      for(uint c = 0; c < vitEqW.size(); ++c){DBGLN;
        if(TryToEquip(vitEqW[c],true)){ DBG1(vitEqW[c]->GetNameSingular().CStr());
          return true;
        }else{DBGLN;
          vitEqW[c]->MoveTo(GetStack()); //was dropped, get back, will be in the end of the stack! :)
        }
      }
    }

    //////////////////////////////// zap
    static int iLastZapTurn=-1;
    if(game::GetTurn()>(iLastZapTurn+100)){ DBG2(game::GetTurn(),iLastZapTurn); //every X turns try to use stuff from inv
      iLastZapTurn=game::GetTurn();

      int iDir=clock()%(8+1); // index 8 is the macro YOURSELF already... if(iDir==8)iDir=YOURSELF;
      static itemvector vitEqW;vitEqW.clear();GetStack()->FillItemVector(vitEqW);
      for(uint c = 0; c < vitEqW.size(); ++c){
        if(!vitEqW[c]->IsZappable(this))continue;

        if(vitEqW[c]->Zap(this, GetPos(), iDir)){ DBG1(vitEqW[c]->GetNameSingular().CStr()); //TODO try to aim at NPCs
          return true;
        }

        if(vitEqW[c]->Apply(this)){ DBG1(vitEqW[c]->GetNameSingular().CStr());
          return true;
        }
      }
    }

    //////////////////////////////// read book
    static int iLastReadTurn=-1;
    if(game::GetTurn()>(iLastReadTurn+50)){ DBG2(game::GetTurn(),iLastReadTurn); //every X turns try to use stuff from inv
      iLastReadTurn=game::GetTurn();

      static itemvector vitEqW;vitEqW.clear();GetStack()->FillItemVector(vitEqW);
      for(uint c = 0; c < vitEqW.size(); ++c){
        if(!vitEqW[c]->IsReadable(this))continue;
        static holybook* hb;hb = dynamic_cast<holybook*>(vitEqW[c]);
        if(hb==NULL)continue;

        if(vitEqW[c]->Read(this)){ DBG1(vitEqW[c]->GetNameSingular().CStr()); //TODO try to aim at NPCs
          return true;
        }
      }
    }
  }

  return false;
}

truth humanoid::CheckIfEquipmentIsNotUsable(int I) const
{
  return (I == RIGHT_WIELDED_INDEX && GetRightArm()->CheckIfWeaponTooHeavy("this item"))
      || (I == LEFT_WIELDED_INDEX && GetLeftArm()->CheckIfWeaponTooHeavy("this item"))
      || (I == RIGHT_WIELDED_INDEX && GetLeftWielded() && GetLeftWielded()->IsTwoHanded()
          && GetLeftArm()->CheckIfWeaponTooHeavy(festring(GetPossessivePronoun() + " other wielded item").CStr()))
      || (I == LEFT_WIELDED_INDEX && GetRightWielded() && GetRightWielded()->IsTwoHanded()
          && GetRightArm()->CheckIfWeaponTooHeavy(festring(GetPossessivePronoun() + " other wielded item").CStr()));
}

int mistress::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, v2 HitPos, double Damage,
                      double ToHitValue, int Success, int Type, int Direction, truth Critical, truth ForceHit)
{
  int Return = humanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage,
                                 ToHitValue, Success, Type, Direction, Critical, ForceHit);

  if(Return == HAS_HIT && Critical)
  {
    if(IsPlayer())
      ADD_MESSAGE("Aahhh. The pain feels unbelievably good.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s screams: \"Oh the delightful pain!\"", CHAR_NAME(DEFINITE));
    else
      ADD_MESSAGE("You hear someone screaming: \"Oh the delightful pain!\"");
  }

  return Return;
}

truth petrusswife::SpecialEnemySightedReaction(character* Char)
{
  item* Weapon = Char->GetMainWielded();

  if(!(GetConfig() == 5) && Weapon && Weapon->IsWeapon(Char) && !(RAND() % 20))
    ADD_MESSAGE("%s screams: \"Oh my Frog, %s's got %s %s!\"",
                CHAR_DESCRIPTION(DEFINITE), Char->CHAR_PERSONAL_PRONOUN_THIRD_PERSON_VIEW,
                Weapon->GetArticle(), Weapon->GetNameSingular().CStr());

  return false;
}

truth housewife::SpecialEnemySightedReaction(character* Char)
{
  item* Weapon = Char->GetMainWielded();

  if(Weapon && Weapon->IsWeapon(Char) && !(RAND() % 5))
    ADD_MESSAGE("%s screams: \"Oh my Frog, %s's got %s %s!\"",
                CHAR_DESCRIPTION(DEFINITE), Char->CHAR_PERSONAL_PRONOUN_THIRD_PERSON_VIEW,
                Weapon->GetArticle(), Weapon->GetNameSingular().CStr());

  return false;
}

void housewife::CreateInitialEquipment(int SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);
  meleeweapon* Weapon;

  if(GetConfig() == CULTIST)
  {
    Weapon = meleeweapon::Spawn(SCYTHE);
    SetRightWielded(Weapon);
    GetCWeaponSkill(POLE_ARMS)->AddHit(100);
    GetCurrentRightSWeaponSkill()->AddHit(50);
    return;
  }
  else switch(RAND() % 4)
  {
   case 0:
    Weapon = meleeweapon::Spawn(ROLLING_PIN);
    SetRightWielded(Weapon);
    GetCWeaponSkill(BLUNT_WEAPONS)->AddHit(50);
    GetCurrentRightSWeaponSkill()->AddHit(10);
    break;
   case 1:
    Weapon = meleeweapon::Spawn(FRYING_PAN);
    SetRightWielded(Weapon);
    GetCWeaponSkill(BLUNT_WEAPONS)->AddHit(50);
    GetCurrentRightSWeaponSkill()->AddHit(10);
    break;
   case 2:
    Weapon = meleeweapon::Spawn(MEAT_CLEAVER);
    SetRightWielded(Weapon);
    GetCWeaponSkill(SMALL_SWORDS)->AddHit(50);
    GetCurrentRightSWeaponSkill()->AddHit(10);
    break;
   default: break;
  }
}

void guard::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << WayPoints << NextWayPoint;
}

void guard::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> WayPoints >> NextWayPoint;
}

void guard::GetAICommand()
{
  if(GetConfig() == MASTER && HP << 1 < MaxHP && (GetPos() - v2(30, 17)).GetLengthSquare() > 9)
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

    GetLevel()->GetLSquare(30, 16)->KickAnyoneStandingHereAway();
    Move(v2(30, 16), true);
    EditAP(-1000);
    return;
  }

  if(WayPoints.size() && !IsGoingSomeWhere())
  {
    if(GetPos() == WayPoints[NextWayPoint])
    {
      if(NextWayPoint < WayPoints.size() - 1)
        ++NextWayPoint;
      else
        NextWayPoint = 0;
    }

    GoingTo = WayPoints[NextWayPoint];
  }

  SeekLeader(GetLeader());

  if(CheckForEnemies(true, true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(FollowLeader(GetLeader()))
    return;

  if(CheckForDoors())
    return;

  if(MoveTowardsHomePos())
    return;

  if(CheckSadism())
    return;

  if(CheckForBeverage())
    return;

  EditAP(-1000);
}

truth mistress::ReceiveDamage(character* Damager, int Damage, int Type, int TargetFlags, int Direction,
                              truth Divide, truth PenetrateArmor, truth Critical, truth ShowMsg)
{
  truth Success = humanoid::ReceiveDamage(Damager, Damage, Type, TargetFlags, Direction,
                                          Divide, PenetrateArmor, Critical, ShowMsg);

  if(Type & SOUND && Success && !(RAND() & 7))
  {
    if(IsPlayer())
      ADD_MESSAGE("Aahhh. The pain feels unbelievably good.");
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s screams: \"Oh the delightful pain!\"", CHAR_NAME(DEFINITE));
    else
      ADD_MESSAGE("You hear someone screaming: \"Oh the delightful pain!\"");
  }

  return Success;
}

void humanoid::AddSpecialStethoscopeInfo(felist& Info) const
{
  Info.AddEntry(CONST_S("Arm strength: ") + GetAttribute(ARM_STRENGTH), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Leg strength: ") + GetAttribute(LEG_STRENGTH), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Dexterity:    ") + GetAttribute(DEXTERITY), LIGHT_GRAY);
  Info.AddEntry(CONST_S("Agility:      ") + GetAttribute(AGILITY), LIGHT_GRAY);
}

item* humanoid::GetPairEquipment(int I) const
{
  switch(I)
  {
   case RIGHT_WIELDED_INDEX: return GetLeftWielded();
   case LEFT_WIELDED_INDEX: return GetRightWielded();
   case RIGHT_GAUNTLET_INDEX: return GetLeftGauntlet();
   case LEFT_GAUNTLET_INDEX: return GetRightGauntlet();
   case RIGHT_BOOT_INDEX: return GetLeftBoot();
   case LEFT_BOOT_INDEX: return GetRightBoot();
  }

  return 0;
}

cfestring& humanoid::GetStandVerb() const
{
  if(ForceCustomStandVerb())
    return DataBase->StandVerb;

  static festring HasntFeet = CONST_S("crawling");
  static festring Hovering = CONST_S("hovering");
  static festring Swimming = CONST_S("swimming");

  if(StateIsActivated(LEVITATION))
    return Hovering;

  if(IsSwimming())
    return Swimming;

  return HasAUsableLeg() ? DataBase->StandVerb : HasntFeet;
}

void darkmage::GetAICommand()
{
  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  character* NearestEnemy = 0;
  long NearestEnemyDistance = 0x7FFFFFFF;
  character* RandomFriend = 0;
  charactervector Friend;
  v2 Pos = GetPos();

  for(int c = 0; c < game::GetTeams(); ++c)
  {
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
    {
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled())
        {
          long ThisDistance = Max<long>(abs(p->GetPos().X - Pos.X), abs(p->GetPos().Y - Pos.Y));

          if((ThisDistance < NearestEnemyDistance
              || (ThisDistance == NearestEnemyDistance && !(RAND() % 3))) && p->CanBeSeenBy(this))
          {
            NearestEnemy = p;
            NearestEnemyDistance = ThisDistance;
          }
        }
    }
    else if(GetTeam()->GetRelation(game::GetTeam(c)) == FRIEND)
    {
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled() && p->CanBeSeenBy(this))
          Friend.push_back(p);
    }
  }

  if(NearestEnemy && NearestEnemy->GetPos().IsAdjacent(Pos))
  {
    if(NearestEnemy->IsSmall()
       && GetAttribute(WISDOM) < NearestEnemy->GetAttackWisdomLimit()
       && !(RAND() % 5)
       && Hit(NearestEnemy, NearestEnemy->GetPos(),
              game::GetDirectionForVector(NearestEnemy->GetPos() - GetPos())))
      return;
    else if((GetConfig() == ARCH_MAGE && RAND() & 1)
            || (GetConfig() == ELDER && !(RAND() & 3)))
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s invokes a spell and disappears.", CHAR_NAME(DEFINITE));

      TeleportRandomly(true);
      EditAP(-GetSpellAPCost());
      return;
    }
  }

  if(NearestEnemy
     && ((GetConfig() != APPRENTICE && NearestEnemyDistance < 10) || StateIsActivated(PANIC))
     && RAND() & 3)
  {
    SetGoingTo((Pos << 1) - NearestEnemy->GetPos());

    if(MoveTowardsTarget(true))
      return;
  }

  if(Friend.size() && !(RAND() & 3))
  {
    RandomFriend = Friend[RAND() % Friend.size()];
    NearestEnemy = 0;
  }

  beamdata Beam
    (
      this,
      CONST_S("killed by the spells of ") + GetName(INDEFINITE),
      YOURSELF,
      0
    );

  if(NearestEnemy)
  {
    lsquare* Square = NearestEnemy->GetLSquareUnder();
    EditAP(-GetSpellAPCost());

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s invokes a spell!", CHAR_NAME(DEFINITE));

    switch(GetConfig())
    {
     case APPRENTICE:
      Square->DrawLightning(v2(8, 8), WHITE, YOURSELF);
      Square->Lightning(Beam);
      break;
     case BATTLE_MAGE:
      if(RAND() % 20)
      {
        Square->DrawLightning(v2(8, 8), WHITE, YOURSELF);
        Square->Lightning(Beam);
      }
      else
      {
        Square->DrawParticles(RED);
        Square->LowerEnchantment(Beam);
      }

      break;
     case ELDER:
      switch(RAND() % 20)
      {
       case 0:
       case 1:
       case 2: Square->DrawParticles(RED); Square->Strike(Beam); break;
       case 3: Square->DrawParticles(RED); Square->FireBall(Beam); break;
       case 4:
       case 5:
       case 6: Square->DrawParticles(RED); Square->Slow(Beam); break;
       case 7: Square->DrawParticles(RED); Square->Teleport(Beam); break;
       case 8:
       case 9:
       case 10: Square->DrawParticles(RED); Square->LowerEnchantment(Beam); break;
       default: Square->DrawLightning(v2(8, 8), WHITE, YOURSELF); Square->Lightning(Beam); break;
      }

      break;
     case ARCH_MAGE:
      switch(RAND() % 20)
      {
       case 0:
       case 1:
       case 2: Square->DrawParticles(RED); Square->FireBall(Beam); break;
       case 3:
        {
          character* Char = NearestEnemy->DuplicateToNearestSquare(this,
            CHANGE_TEAM|MIRROR|(1000 << LE_BASE_SHIFT)|(1000 << LE_RAND_SHIFT));

          if(Char)
          {
            if(Char->CanBeSeenByPlayer())
              ADD_MESSAGE("%s materializes!", Char->CHAR_NAME(INDEFINITE));

            break;
          }
        }
       case 4:
       case 5: Square->DrawParticles(RED); Square->Slow(Beam); break;
       case 6: Square->DrawParticles(RED); Square->Teleport(Beam); break;
       case 7:
       case 8:
       case 9: Square->DrawParticles(RED); Square->LowerEnchantment(Beam); break;
       case 10:
        {
          golem* Golem = golem::Spawn(RAND() % 3 ? ARCANITE : OCTIRON);
          v2 Where = GetLevel()->GetNearestFreeSquare(Golem, Square->GetPos());

          if(Where == ERROR_V2)
          {
            if(CanBeSeenByPlayer())
              ADD_MESSAGE("Nothing happens.");

            delete Golem;
          }
          else
          {
            Golem->SetGenerationDanger(GetGenerationDanger());
            Golem->SetTeam(GetTeam());
            Golem->PutTo(Where);

            if(Golem->CanBeSeenByPlayer())
              ADD_MESSAGE("Suddenly %s materializes!", Golem->CHAR_NAME(INDEFINITE));

            Golem->GetLSquareUnder()->DrawParticles(RED);
          }

          break;
        }
       default: Square->DrawParticles(RED); Square->Strike(Beam); break;
      }

      break;
    }

    if(CanBeSeenByPlayer())
      NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell of ") + GetName(DEFINITE)
                                              + CONST_S(" interrupts you."));
    else
      NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell interrupts you."));

    return;
  }

  if(RandomFriend)
  {
    lsquare* Square = RandomFriend->GetLSquareUnder();
    EditAP(-GetSpellAPCost());
    Square->DrawParticles(RED);

    switch(GetConfig())
    {
     case APPRENTICE:
     case BATTLE_MAGE:
      Square->Haste(Beam);
      break;
     case ARCH_MAGE:
      if(!(RAND() & 31))
      {
        RandomFriend->DuplicateToNearestSquare(this, CHANGE_TEAM);
        return;
      }
     case ELDER:
      if(RAND() & 1)
        Square->Invisibility(Beam);
      else
        Square->Haste(Beam);

      break;
    }

    return;
  }

  if(CheckForDoors())
    return;

  if(CheckSadism())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

void zombie::GetAICommand()
{
  if(!GetHead())
  {
    for(stackiterator i = GetLSquareUnder()->GetStack()->GetBottom(); i.HasItem(); ++i)
    {
      head* Head = i->Behead();

      if(Head)
      {
        if(CanBeSeenByPlayer())
          ADD_MESSAGE("%s takes %s and attaches it to its torso.",
                      CHAR_NAME(DEFINITE), Head->CHAR_NAME(INDEFINITE));

        Head->RemoveFromSlot();
        AttachBodyPart(Head);
        Head->SetHP(1);
        DexterityAction(10);
        return;
      }
    }
  }

  humanoid::GetAICommand();
}

head* humanoid::Behead()
{
  head* Head = GetHead();

  if(Head)
    SevereBodyPart(HEAD_INDEX);

  return Head;
}

truth communist::BoundToUse(citem* Item, int I) const
{
  return Item && Item->IsGorovitsFamilyRelic() && Item->IsInCorrectSlot(I);
}

festring werewolfwolf::GetKillName() const
{
  if(GetPolymorphBackup() && GetPolymorphBackup()->GetType() == werewolfhuman::ProtoType.GetIndex())
    return GetName(INDEFINITE);

  return humanoid::GetKillName();
}

truth werewolfwolf::SpecialBiteEffect(character* Victim, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour, truth Critical, int DoneDamage)
{
  if(!BlockedByArmour && Victim->IsWarmBlooded() && (!(RAND() % 2) || Critical) && !Victim->AllowSpoil())
  {
    // Werewolf wolf gives lycanthropy
    if(Victim->IsHumanoid() && !Victim->StateIsActivated(VAMPIRISM) && !Victim->StateIsActivated(LYCANTHROPY) && !Victim->StateIsActivated(DISEASE_IMMUNITY))
      Victim->BeginTemporaryState(LYCANTHROPY, 6000 + RAND_N(2000));

    // Werewolves do double damage against vampires and this is a drain attack
    if(Victim->StateIsActivated(VAMPIRISM) && (DoneDamage >= 1))
    {
      if(IsPlayer())
        ADD_MESSAGE("You drain some life force from %s!", Victim->CHAR_DESCRIPTION(DEFINITE));
      else if(Victim->IsPlayer() || Victim->CanBeSeenByPlayer() || CanBeSeenByPlayer())
        ADD_MESSAGE("%s drains some life force from %s!", CHAR_DESCRIPTION(DEFINITE), Victim->CHAR_DESCRIPTION(DEFINITE));

      return Victim->ReceiveBodyPartDamage(this, DoneDamage, DRAIN, BodyPartIndex, Direction);
    }
    else
      return false;
  }
  else
    return false;
}

int humanoid::GetRandomApplyBodyPart() const
{
  if(RightArmIsUsable())
  {
    if(LeftArmIsUsable())
      return RAND_2 ? RIGHT_ARM_INDEX : LEFT_ARM_INDEX;
    else
      return RIGHT_ARM_INDEX;
  }
  else if(LeftArmIsUsable())
    return LEFT_ARM_INDEX;

  if(GetHead())
    return HEAD_INDEX;

  return TORSO_INDEX;
}

void golem::BeTalkedTo()
{
  static long Said;

  if(GetRelation(PLAYER) == HOSTILE)
    Engrave(GetHostileReplies()[RandomizeReply(Said, GetHostileReplies().Size)]);
  else
    Engrave(GetFriendlyReplies()[RandomizeReply(Said, GetFriendlyReplies().Size)]);

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s engraves something.", CHAR_NAME(DEFINITE));
}

#ifdef WIZARD

void humanoid::AddAttributeInfo(festring& Entry) const
{
  Entry.Resize(42);
  Entry << GetAttribute(ARM_STRENGTH);
  Entry.Resize(45);
  Entry << GetAttribute(LEG_STRENGTH);
  Entry.Resize(48);
  Entry << GetAttribute(DEXTERITY);
  Entry.Resize(51);
  Entry << GetAttribute(AGILITY);
  character::AddAttributeInfo(Entry);
}

void humanoid::AddAttackInfo(felist& List) const
{
  if(GetAttackStyle() & USE_ARMS)
  {
    if(GetRightArm())
      GetRightArm()->AddAttackInfo(List);

    if(GetLeftArm())
      GetLeftArm()->AddAttackInfo(List);
  }

  festring Entry;

  if(IsUsingLegs())
  {
    GetRightLeg()->AddAttackInfo(List);
    GetLeftLeg()->AddAttackInfo(List);
  }

  if(IsUsingHead())
  {
    Entry = CONST_S("   bite attack");
    Entry.Resize(50);
    Entry << GetHead()->GetBiteMinDamage() << '-' << GetHead()->GetBiteMaxDamage();
    Entry.Resize(60);
    Entry << int(GetHead()->GetBiteToHitValue());
    Entry.Resize(70);
    Entry << GetHead()->GetBiteAPCost();
    List.AddEntry(Entry, LIGHT_GRAY);
  }
}

void humanoid::AddDefenceInfo(felist& List) const
{
  character::AddDefenceInfo(List);

  if(GetRightArm())
    GetRightArm()->AddDefenceInfo(List);

  if(GetLeftArm())
    GetLeftArm()->AddDefenceInfo(List);
}

void humanoid::DetachBodyPart()
{
  int ToBeDetached;

  switch(game::KeyQuestion(CONST_S("What limb? (l)eft arm, (r)ight arm, (L)eft leg, (R)ight leg, (h)ead?"),
                           KEY_ESC, 5, 'l', 'r', 'L', 'R', 'h'))
  {
   case 'l':
    ToBeDetached = LEFT_ARM_INDEX;
    break;
   case 'r':
    ToBeDetached = RIGHT_ARM_INDEX;
    break;
   case 'L':
    ToBeDetached = LEFT_LEG_INDEX;
    break;
   case 'R':
    ToBeDetached = RIGHT_LEG_INDEX;
    break;
   case 'h':
    ToBeDetached = HEAD_INDEX;
    break;
   default:
    return;
  }

  if(GetBodyPart(ToBeDetached))
  {
    item* ToDrop = SevereBodyPart(ToBeDetached);
    SendNewDrawRequest();

    if(ToDrop)
    {
      GetStack()->AddItem(ToDrop);
      ToDrop->DropEquipment();
    }

    ADD_MESSAGE("Bodypart detached!");
  }
  else
    ADD_MESSAGE("That bodypart has already been detached.");

  CheckDeath(CONST_S("removed one of his vital bodyparts"), 0);
}

void humanoid::SetFireToBodyPart()
{
  int ToBeSetFireTo;

  switch(game::KeyQuestion(CONST_S("What limb? (l)eft arm, (r)ight arm, (L)eft leg, (R)ight leg, (h)ead?"),
                           KEY_ESC, 5, 'l', 'r', 'L', 'R', 'h'))
  {
   case 'l':
    ToBeSetFireTo = LEFT_ARM_INDEX;
    break;
   case 'r':
    ToBeSetFireTo = RIGHT_ARM_INDEX;
    break;
   case 'L':
    ToBeSetFireTo = LEFT_LEG_INDEX;
    break;
   case 'R':
    ToBeSetFireTo = RIGHT_LEG_INDEX;
    break;
   case 'h':
    ToBeSetFireTo = HEAD_INDEX;
    break;
   default:
    return;
  }

  if(GetBodyPart(ToBeSetFireTo))
  {
    IgniteBodyPart(ToBeSetFireTo, game::NumberQuestion(CONST_S("How much fire damage?"), PINK));
    SendNewDrawRequest();

  }
  else
    ADD_MESSAGE("That bodypart has previously been detached.");

  CheckDeath(CONST_S("burnt off one of his vital bodyparts"), 0);
}

#else

void humanoid::AddAttributeInfo(festring&) const { }
void humanoid::AddAttackInfo(felist&) const { }
void humanoid::AddDefenceInfo(felist&) const { }
void humanoid::DetachBodyPart() { }
void humanoid::SetFireToBodyPart() { }

#endif

truth ennerbeast::MustBeRemovedFromBone() const
{
  return !IsEnabled()
    || GetTeam()->GetID() != MONSTER_TEAM
    || GetDungeon()->GetIndex() != ELPURI_CAVE
    || GetLevel()->GetIndex() != ENNER_BEAST_LEVEL;
}

truth ennerchild::MustBeRemovedFromBone() const
{
  return !IsEnabled()
    || GetTeam()->GetID() != MONSTER_TEAM
    || GetDungeon()->GetIndex() != XINROCH_TOMB
    || GetLevel()->GetIndex() != DUAL_ENNER_BEAST_LEVEL;
}

truth child::MustBeRemovedFromBone() const
{
  if(GetConfig() != KING)
    return false;

  return !IsEnabled()
    || GetTeam()->GetID() != ASLONA_TEAM
    || GetDungeon()->GetIndex() != GOBLIN_FORT
    || GetLevel()->GetIndex() != KING_LEVEL;
}

truth communist::MustBeRemovedFromBone() const
{
  return !IsEnabled()
    || GetTeam()->GetID() != IVAN_TEAM
    || GetDungeon()->GetIndex() != ELPURI_CAVE
    || GetLevel()->GetIndex() != IVAN_LEVEL;
}

truth humanoid::PreProcessForBone()
{
  for(sweaponskill* p : SWeaponSkill)
    p->PreProcessForBone();

  return character::PreProcessForBone();
}

void humanoid::FinalProcessForBone()
{
  character::FinalProcessForBone();

  for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end();)
  {
    boneidmap::iterator BI = game::GetBoneItemIDMap().find(-(*i)->GetID());

    if(BI == game::GetBoneItemIDMap().end())
    {
      std::list<sweaponskill*>::iterator Dirt = i++;
      SWeaponSkill.erase(Dirt);
    }
    else
    {
      (*i)->SetID(BI->second);
      ++i;
    }
  }
}

void petrus::FinalProcessForBone()
{
  humanoid::FinalProcessForBone();
  LastHealed = 0;
}

void angel::FinalProcessForBone()
{
  humanoid::FinalProcessForBone();
  LastHealed = 0;
}

/*void encourager::FinalProcessForBone()
{
  humanoid::FinalProcessForBone();
  LastHit = 0;
}*/

void playerkind::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << SoulID << HairColor << EyeColor << Talent << Weakness << IsBonePlayer << IsClone;
}

void playerkind::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> SoulID >> HairColor >> EyeColor >> Talent >> Weakness >> IsBonePlayer >> IsClone;
}

void playerkind::SetSoulID(ulong What)
{
  SoulID = What;

  if(GetPolymorphBackup())
    GetPolymorphBackup()->SetSoulID(What);
}

truth playerkind::SuckSoul(character* Soul)
{
  if(Soul->GetID() == SoulID)
  {
    SoulID = 0;
    return true;
  }

  return false;
}

truth playerkind::TryToRiseFromTheDead()
{
  if(humanoid::TryToRiseFromTheDead())
  {
    if(IsEnabled() && SoulID)
    {
      ADD_MESSAGE("The soulless body of %s wobbles for a moment.", CHAR_NAME(DEFINITE));
      return false;
    }

    return true;
  }
  else
    return false;
}

void playerkind::FinalProcessForBone()
{
  humanoid::FinalProcessForBone();
  IsBonePlayer = true;

  if(SoulID)
  {
    boneidmap::iterator BI = game::GetBoneCharacterIDMap().find(SoulID);

    if(BI != game::GetBoneCharacterIDMap().end())
      SoulID = BI->second;
    else
      SoulID = 0;
  }
}

playerkind::playerkind(const playerkind& Char)
: mybase(Char), SoulID(Char.SoulID), HairColor(Char.HairColor), EyeColor(Char.EyeColor),
  Talent(Char.Talent), Weakness(Char.Weakness), IsBonePlayer(Char.IsBonePlayer), IsClone(true)
{
}

void playerkind::BeTalkedTo()
{
  if(IsClone && IsBonePlayer)
  {
    if(GetRelation(PLAYER) == HOSTILE)
    {
      ADD_MESSAGE("Oh no, you too! Why does everyone bully me!");
      return;
    }

    static long Said;

    switch(RandomizeReply(Said, 4))
    {
     case 0:
      ADD_MESSAGE("\"I'd like to write a memoir, but alas I doubt anyone would believe it.\"");
      break;
     case 1:
      ADD_MESSAGE("\"Then that damned clone appeared, took all my equipment and claimed I was his slave...\"");
      break;
     case 2:
      ADD_MESSAGE("\"The level was a catastrophe for the party, but luckily you saved the day.\"");
      break;
     case 3:
      ADD_MESSAGE("\"Oh, how I hate bananas. I Hate Them! I HATE THEM SO MUCH!!!\"");
      break;
    }
  }
  else if(IsClone)
  {
    if(GetRelation(PLAYER) == HOSTILE)
    {
      ADD_MESSAGE("%s seems extremely irritated. \"Vanish, you foul mirror image!\"", CHAR_DESCRIPTION(DEFINITE));
      return;
    }

    static long Said;

    switch(RandomizeReply(Said, 4))
    {
     case 0:
      ADD_MESSAGE("\"Hey, those clothes are mine! Give them back!\"");
      break;
     case 1:
      ADD_MESSAGE("\"What, you summoned me? What a coincidence, I remember summoning you, too.\"");
      break;
     case 2:
      ADD_MESSAGE("\"I'm leading this party, not you, Mr. copy guy!\"");
      break;
     case 3:
      ADD_MESSAGE("\"Oh, how I hate bananas. I Hate Them! I HATE THEM SO MUCH!!!\"");
      break;
    }
  }
  else
  {
    if(GetRelation(PLAYER) == HOSTILE)
    {
      ADD_MESSAGE("Let's finish what my ghost failed to do!");
      return;
    }

    static long Said;

    switch(RandomizeReply(Said, 4))
    {
     case 0:
      ADD_MESSAGE("\"What was it like? Death, you mean? Well, just like New Attnam. Very hot and whips everywhere.\"");
      break;
     case 1:
      ADD_MESSAGE("\"Stop it already! I *don't* want to know how my corpse smelled!\"");
      break;
     case 2:
      ADD_MESSAGE("\"I'm sorry about that ghost thing. That YASD was just a bit too much to handle, so I lost myself.\"");
      break;
     case 3:
      ADD_MESSAGE("\"Oh, how I hate bananas. I Hate Them! I HATE THEM SO MUCH!!!\"");
      break;
    }
  }
}

void humanoid::EnsureCurrentSWeaponSkillIsCorrect(sweaponskill*& Skill, citem* Wielded)
{
  if(Wielded)
  {
    if(!Skill || !Skill->IsSkillOf(Wielded))
    {
      if(Skill)
        EnsureCurrentSWeaponSkillIsCorrect(Skill, 0);

      for(sweaponskill* p : SWeaponSkill)
        if(p->IsSkillOf(Wielded))
        {
          Skill = p;
          return;
        }

      for(idholder* I = Wielded->GetCloneMotherID(); I; I = I->Next)
        for(sweaponskill* p : SWeaponSkill)
          if(p->IsSkillOfCloneMother(Wielded, I->ID))
          {
            Skill = new sweaponskill(*p);
            Skill->SetID(Wielded->GetID());
            SWeaponSkill.push_back(Skill);
            return;
          }

      Skill = new sweaponskill(Wielded);
      SWeaponSkill.push_back(Skill);
    }
  }
  else if(Skill)
  {
    if(!Skill->GetHits() && (CurrentRightSWeaponSkill != Skill || CurrentLeftSWeaponSkill != Skill))
      for(std::list<sweaponskill*>::iterator i = SWeaponSkill.begin(); i != SWeaponSkill.end(); ++i)
        if(*i == Skill)
        {
          delete *i;
          SWeaponSkill.erase(i);
          break;
        }

    Skill = 0;
  }
}

humanoid::~humanoid()
{
  for(sweaponskill* p : SWeaponSkill)
    delete p;
}

truth petrus::MoveTowardsHomePos()
{
  if(GetPos() != v2(28, 20))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

    GetLevel()->GetLSquare(28, 20)->KickAnyoneStandingHereAway();
    Move(v2(28, 20), true);

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s appears.", CHAR_NAME(DEFINITE));

    EditAP(-1000);
    return true;
  }
  else
    return false;
}

truth guard::MoveTowardsHomePos()
{
  if(GetConfig() == MASTER && GetPos() != v2(30, 16))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s disappears.", CHAR_NAME(DEFINITE));

    GetLevel()->GetLSquare(30, 16)->KickAnyoneStandingHereAway();
    Move(v2(30, 16), true);

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s appears.", CHAR_NAME(DEFINITE));

    EditAP(-1000);
    return true;
  }
  else
    return humanoid::MoveTowardsHomePos();
}

bodypart* ennerbeast::MakeBodyPart(int I) const
{
  if(I == HEAD_INDEX)
    return ennerhead::Spawn(0, NO_MATERIALS);
  else
    return humanoid::MakeBodyPart(I);
}

bodypart* ennerchild::MakeBodyPart(int I) const
{
  if(I == HEAD_INDEX)
    return ennerhead::Spawn(0, NO_MATERIALS);
  else
    return humanoid::MakeBodyPart(I);
}

int humanoid::GetSumOfAttributes() const
{
  return character::GetSumOfAttributes() + GetAttribute(LEG_STRENGTH) + GetAttribute(DEXTERITY) ;
}

truth humanoid::CheckConsume(cfestring& Verb) const
{
  if(!HasHead())
  {
    if(IsPlayer())
      ADD_MESSAGE("You need a head to %s.", Verb.CStr());

    return false;
  }

  return character::CheckConsume(Verb);
}

truth humanoid::CanConsume(material* Material) const
{
  return character::CanConsume(Material) && HasHead();
}

void femaleslave::BeTalkedTo()
{
  static long Said;

  if(GetConfig() != NEW_ATTNAM || GetRelation(PLAYER) == HOSTILE)
    humanoid::BeTalkedTo();
  else if(!game::TweraifIsFree())
    ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, 4)]);
  else
    ProcessAndAddMessage(GetFriendlyReplies()[4 + RandomizeReply(Said, 3)]);
}

void necromancer::GetAICommand()
{
  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  character* NearestEnemy = 0;
  long NearestEnemyDistance = 0x7FFFFFFF;
  v2 Pos = GetPos();

  for(int c = 0; c < game::GetTeams(); ++c)
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
    {
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled())
        {
          long ThisDistance = Max<long>(abs(p->GetPos().X - Pos.X), abs(p->GetPos().Y - Pos.Y));

          if((ThisDistance < NearestEnemyDistance
              || (ThisDistance == NearestEnemyDistance && !(RAND() % 3))) && p->CanBeSeenBy(this))
          {
            NearestEnemy = p;
            NearestEnemyDistance = ThisDistance;
          }
        }
    }

  if(NearestEnemy && NearestEnemy->GetPos().IsAdjacent(Pos))
  {
    if(GetConfig() != APPRENTICE_NECROMANCER && !(RAND() & 3))
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s invokes a spell and disappears.", CHAR_NAME(DEFINITE));

      TeleportRandomly(true);
      EditAP(-GetSpellAPCost());
      return;
    }
    else if(NearestEnemy->IsSmall()
            && GetAttribute(WISDOM) < NearestEnemy->GetAttackWisdomLimit()
            && !(RAND() & 3)
            && Hit(NearestEnemy,
                   NearestEnemy->GetPos(),
                   game::GetDirectionForVector(NearestEnemy->GetPos() - GetPos())))
      return;
  }

  if(!NearestEnemy)
  {
    if(!RAND_N(3) && TryToRaiseZombie())
      return;
  }
  else
  {
    if(!RAND_N(6) && TryToRaiseZombie())
      return;
  }

  if(NearestEnemy && !(RAND() % (GetConfig() == APPRENTICE_NECROMANCER ? 3 : 2)))
  {
    lsquare* Square = NearestEnemy->GetLSquareUnder();
    EditAP(-GetSpellAPCost());

    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s invokes a spell!", CHAR_NAME(DEFINITE));

    truth Interrupt = false;

    switch(GetConfig())
    {
     case APPRENTICE_NECROMANCER:
      RaiseSkeleton();
      break;
     default:
      if(RAND() % 5)
        RaiseSkeleton();
      else
      {
        Square->DrawLightning(v2(8, 8), WHITE, YOURSELF);

        beamdata Beam
          (
            this,
            CONST_S("killed by the spells of ") + GetName(INDEFINITE),
            YOURSELF,
            0
          );

        Square->Lightning(Beam);
        Interrupt = true;
      }

      break;
    }

    if(Interrupt)
    {
      if(CanBeSeenByPlayer())
        NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell of ") + GetName(DEFINITE)
                                                + CONST_S(" interrupts you."));
      else
        NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell interrupts you."));
    }

    return;
  }

  if(NearestEnemy && (NearestEnemyDistance < 10 || StateIsActivated(PANIC)) && RAND() & 3)
  {
    SetGoingTo((Pos << 1) - NearestEnemy->GetPos());

    if(MoveTowardsTarget(true))
      return;
  }

  if(GetConfig() == IMPRISONED_NECROMANCER && !(GetRelation(PLAYER) == HOSTILE))
  {
    humanoid::MoveTowardsHomePos();
    return;
  }

  if(CheckForDoors())
    return;

  if(CheckSadism())
    return;

  if(MoveRandomly())
    return;

  EditAP(-1000);
}

truth necromancer::TryToRaiseZombie()
{
  for(int c = 0; c < game::GetTeams(); ++c)
    for(character* p : game::GetTeam(c)->GetMember())
      if(!p->IsEnabled() && p->GetMotherEntity()
         && p->GetMotherEntity()->Exists()
         && (GetConfig() == MASTER_NECROMANCER
             || p->GetMotherEntity()->GetSquareUnderEntity()->CanBeSeenBy(this)))
      {
        character* Zombie = p->GetMotherEntity()->TryNecromancy(this);

        if(Zombie)
        {
          if(Zombie->CanBeSeenByPlayer())
            ADD_MESSAGE("%s calls %s back to cursed undead life.",
                        CHAR_DESCRIPTION(DEFINITE), Zombie->CHAR_NAME(INDEFINITE));
          else if(CanBeSeenByPlayer())
            ADD_MESSAGE("%s casts a spell, but you notice no effect.",
                        CHAR_NAME(DEFINITE));

          EditAP(-GetSpellAPCost());
          return true;
        }
      }

  return false;
}

void necromancer::RaiseSkeleton()
{
  /* Gum solution */

  const database* WarLordDataBase;
  databasecreator<character>::FindDataBase(WarLordDataBase, &skeleton::ProtoType, WAR_LORD);
  skeleton* Skeleton;

  if(GetConfig() == MASTER_NECROMANCER && !(WarLordDataBase->Flags & HAS_BEEN_GENERATED) && !(game::GetCurrentDungeonIndex() == XINROCH_TOMB) && !(RAND() % 250))
  {
    Skeleton = skeleton::Spawn(WAR_LORD);
    Skeleton->SetTeam(GetTeam());
    Skeleton->PutNear(GetPos());
    Skeleton->SignalGeneration();

    if(Skeleton->CanBeSeenByPlayer())
      ADD_MESSAGE("The whole area trembles terribly as %s emerges from the ground.", Skeleton->CHAR_NAME(DEFINITE));
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s casts a powerful spell which makes the whole area tremble.", CHAR_NAME(DEFINITE));
    else
      ADD_MESSAGE("You feel the presence of an ancient evil being awakened from its long slumber. You shiver.");
  }
  else
  {
    Skeleton = skeleton::Spawn(GetConfig() == APPRENTICE_NECROMANCER ? 0 : WARRIOR, NO_EQUIPMENT);
    Skeleton->SetTeam(GetTeam());
    Skeleton->PutNear(GetPos());

    if(Skeleton->CanBeSeenByPlayer())
      ADD_MESSAGE("The ground shakes and %s emerges from it.", Skeleton->CHAR_NAME(INDEFINITE));
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s casts a spell, but you notice no effect.", CHAR_NAME(DEFINITE));
  }

  Skeleton->SetGenerationDanger(GetGenerationDanger());
  EditAP(-GetSpellAPCost());
}

void necromancer::BeTalkedTo()
{
  if(GetConfig() != IMPRISONED_NECROMANCER || !GetPos().IsAdjacent(PLAYER->GetPos()))
  {
    humanoid::BeTalkedTo();
    return;
  }

  /* From here we are talking to the necromancer in the attnamese catacombs */
  if(GetRelation(PLAYER) == HOSTILE)
  {
    ADD_MESSAGE("\"I will bury you in the catacombs with all the others!\"");
    return;
  }

  if(game::GetXinrochTombStoryState() == 1)
  {
    if(PLAYER->HasShadowVeil() && PLAYER->RemoveShadowVeil(this))
    {
      game::TextScreen(CONST_S("\"At last I can make my escape from Petrus' wretched clutches!\"\n\n"
                               "Anmah takes the shadow veil from you and seems completely lost in\n"
                               "thoughts for a while. Suddenly, he looks up:\n\n"
                               "\"Oh, you are still here. Good! Pray tell me, what did you find in the Tomb?\n"
                               "A portal? Did you traverse it? Of course not! You can't do so bodily,\n"
                               "unless you were...\n\n...changed in some way.\"\n\n"
                               "Before you can stop him, he reaches for your face."));

      game::TextScreen(CONST_S("You feel a cold, tingling sensation in the middle of your forehead.\n\n"
                               "\"Here, I give you the seal of the undead. You will be able to traverse\n"
                               "the portal without the use of the shadow veil. You can still retrieve\n"
                               "the lost flaming ruby sword. If you go beyond the portal, you will find\n"
                               "the one who carries this lost sword. But be warned, he is a terrible foe!\""));

      GetArea()->SendNewDrawRequest();
      ADD_MESSAGE("\"May Infuscor guide you towards the darkest of secrets.\"");
      game::SetXinrochTombStoryState(2);
    }
    else
    {
      ADD_MESSAGE("%s says: \"Bring me the shadow veil and we'll talk.\"", CHAR_NAME(DEFINITE));
      return;
    }
  }

  if(PLAYER->HasLostRubyFlamingSword())
  {
    ADD_MESSAGE("%s exclaims: \"What are you still doing down here? That sword belongs to the Champion of Infuscor!\"", CHAR_NAME(DEFINITE));
    return;
  }
  else if(game::GetXinrochTombStoryState() == 2)
    ADD_MESSAGE("%s says: \"I am just preparing to leave. Have you found that flaming ruby sword yet?\"", CHAR_NAME(DEFINITE));

  if(game::GetStoryState() == 1)
  {
    ADD_MESSAGE("%s looks up: \"Would you mind helping me with a little problem?\"", CHAR_NAME(DEFINITE));

    if(game::TruthQuestion(CONST_S("Do you accept the quest? [y/N]"), REQUIRES_ANSWER))
    {
      /*game::TextScreen(CONST_S("The necromancer takes the scroll and mutters an incantation in a low voice.\n"
                               "To your surprise, the words rearrange themselves on the page,\n"
                               "revealing a previously inscrutable message.\n"
                               "The necromancer scans the page from left to right several times. His face contorts:\n"
                               "\"Bah! A canticle of Saint Petrus the Lion-Hearted!\"\n"
                               "He continues down the page. His eyes widen:\n"
                               "\"O ho! 10 000 bananas? It sounds bad out in the colonies. I'm sorry to hear about it.\"\n"
                               "\n"
                               "The necromancer allows the scroll to burn and the ashes wither away in his hands.\n"
                               "\"Alas, no news about my trial. But thank you for sharing.\"\n\n"
      */
      game::TextScreen(CONST_S("\"You might be asking what am I doing down here? Lets just say I had spent some time\n"
                               "arranging... things in the catacombs below. I was the undertaker for the city of Attnam,\n"
                               "you see. Well, curiosity got the better of me and I admit I dabbled in some necromancy.\n"
                               "223 years later, and I was still down here, drinking blood, eating bones, and generally \n"
                               "trying all the old life-extension tricks. Finally I got caught by that meddling Haedlac.\n"
                               "He's got nothing better to do these days, I guess. He sent me here to the Cellar, agonizingly\n"
                               "close to my minions, but still unable to escape. That stupid floating eye hovers by here\n"
                               "every now and again to check up on me.\""));

      game::TextScreen(CONST_S("\"Wait, don't go yet! It gets lonely here, with no one to talk to but the punishers.\n"
                               "Keep me company a little longer, please... Maybe I can tell you a story? I can relate\n"
                               "the history of dark knighthood to you.\"\n\n"
                               "\"Long ago, there lived a powerful warrior, Xinroch, who rose up the ranks\n"
                               "of the fearsome order of the dark knights, to become the grand master dark knight. \n\n"
                               "His soul dwells within his mausoleum, not far from here. He doesn't stand a chance\n"
                               "of returning to us; not without a piece of his soul getting out. There is a cadre\n"
                               "of devoted dark knights, called the Templars. Being eager to protect the resting place\n"
                               "of their legendary master, they may obstruct your entry to the tomb. Little do they know\n"
                               "that in order for their master to be reborn, his spirit must be freed from the place.\n"
                               "Of course, disturbing such a restless soul would be dangerous. You may need to subdue it\n"
                               "by force to gain what you need. Legend has it Xinroch's spirit is able to wield weapons,\n"
                               "and possesses a cloak of unimaginable usefulness: The Shadow Veil.\""));

      game::TextScreen(CONST_S("The necromancer suddenly looks at you intently.\n\n"
                               "\"Okay, we can talk now. The cardinals are not listening to our thoughts. I need you to\n"
                               "bring me the shadow veil, it will surely allow me to escape from Attnam. It has certain\n"
                               "properties conducive to getting away unnoticed.\"\n\n"
                               "\"It will take all your wits to survive the powers of the Tomb of Xinroch, but I believe\n"
                               "in you. You are my only hope. Oh, how I wish to taste fresh blood again!\"\n\n"
                               "\"Lastly, there is the matter of Xinroch's lost sword. Its power lies in its symbolism.\n"
                               "If you were to gain it somehow, then I imagine most believers would be convinced that\n"
                               "you were Xinroch himself, returned to the flesh. Although you would need to prove this\n"
                               "with the help of our god, Infuscor... ...it might require some offering, perhaps? I have\n"
                               "a feeling that if you find anything belonging to Xinroch, it will help you greatly\n"
                               "in your quest.\"\n\n"
                               "\"I cannot say what trial would await you to retrieve the lost sword, but I'm sure\n"
                               "a mighty adventurer like you would like to lead a whole order of dark knights?\""));

      game::LoadWorldMap();
      v2 XinrochTombPos = game::GetWorldMap()->GetEntryPos(0, XINROCH_TOMB);
      game::GetWorldMap()->GetWSquare(XinrochTombPos)->ChangeOWTerrain(xinrochtomb::Spawn());
      game::GetWorldMap()->RevealEnvironment(XinrochTombPos, 1);
      game::SaveWorldMap();
      GetArea()->SendNewDrawRequest();
      ADD_MESSAGE("\"And don't worry about the patrol guard, he's not really paying attention to who leaves the Attnam or if they should be leaving.\"");
      game::SetXinrochTombStoryState(1);
      game::SetStoryState(2);
      return;
    }
    else
    {
      ADD_MESSAGE("%s looks downcast. \"I see. I guess I shall have to wait for another adventurer then.\"", CHAR_NAME(DEFINITE));
      return;
    }
  }
  else
    ADD_MESSAGE("%s says: \"Come back when you are on no other quests.\"", CHAR_NAME(DEFINITE));
}

void humanoid::StayOn(liquid* Liquid)
{
  if(IsFlying())
    return;

  truth Standing = false;

  if(GetRightLeg())
  {
    GetRightLeg()->StayOn(Liquid);
    Standing = true;
  }

  if(IsEnabled() && GetLeftLeg())
  {
    GetLeftLeg()->StayOn(Liquid);
    Standing = true;
  }

  if(!Standing)
  {
    bodypart* BodyPart[MAX_BODYPARTS];
    int Index = 0;

    for(int c = 0; c < BodyParts; ++c)
      if(GetBodyPart(c))
        BodyPart[Index++] = GetBodyPart(c);

    BodyPart[RAND() % Index]->StayOn(Liquid);
  }
}

bodypart* playerkind::MakeBodyPart(int I) const
{
  switch(I)
  {
   case TORSO_INDEX: return playerkindtorso::Spawn(0, NO_MATERIALS);
   case HEAD_INDEX: return playerkindhead::Spawn(0, NO_MATERIALS);
   case RIGHT_ARM_INDEX: return playerkindrightarm::Spawn(0, NO_MATERIALS);
   case LEFT_ARM_INDEX: return playerkindleftarm::Spawn(0, NO_MATERIALS);
   case GROIN_INDEX: return playerkindgroin::Spawn(0, NO_MATERIALS);
   case RIGHT_LEG_INDEX: return playerkindrightleg::Spawn(0, NO_MATERIALS);
   case LEFT_LEG_INDEX: return playerkindleftleg::Spawn(0, NO_MATERIALS);
  }

  ABORT("Weird bodypart to make for a playerkind. It must be your fault!");
  return 0;
}

truth golem::AddAdjective(festring& String, truth Articled) const
{
  int TotalRustLevel = sumbodypartproperties()(this, &bodypart::GetMainMaterialRustLevel);

  if(!TotalRustLevel)
    return humanoid::AddAdjective(String, Articled);
  else
  {
    if(Articled)
      String << "a ";

    if(TotalRustLevel <= GetBodyParts())
      String << "slightly rusted ";
    else if(TotalRustLevel <= GetBodyParts() << 1)
      String << "rusted ";
    else
      String << "very rusted ";

    String << GetAdjective() << ' ';
    return true;
  }
}

void oree::Bite(character* Enemy, v2 HitPos, int, truth)
{
  if(IsPlayer())
    ADD_MESSAGE("You vomit acidous blood at %s.", Enemy->CHAR_DESCRIPTION(DEFINITE));
  else if(Enemy->IsPlayer() || CanBeSeenByPlayer() || Enemy->CanBeSeenByPlayer())
    ADD_MESSAGE("%s vomits acidous blood at %s.", CHAR_DESCRIPTION(DEFINITE), Enemy->CHAR_DESCRIPTION(DEFINITE));

  Vomit(HitPos, 500 + RAND() % 500, false);
}

truth vampire::SpecialBiteEffect(character* Victim, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour, truth Critical, int DoneDamage)
{
  if(!BlockedByArmour && Victim->IsWarmBlooded() && (!(RAND() % 2) || Critical) && !Victim->AllowSpoil())
  {
    if(IsPlayer())
      ADD_MESSAGE("You drain some precious lifeblood from %s!", Victim->CHAR_DESCRIPTION(DEFINITE));
    else if(Victim->IsPlayer() || Victim->CanBeSeenByPlayer() || CanBeSeenByPlayer())
      ADD_MESSAGE("%s drains some precious lifeblood from %s!", CHAR_DESCRIPTION(DEFINITE), Victim->CHAR_DESCRIPTION(DEFINITE));

    if(Victim->IsHumanoid() && !Victim->StateIsActivated(VAMPIRISM) && !Victim->StateIsActivated(LYCANTHROPY) && !Victim->StateIsActivated(DISEASE_IMMUNITY))
      Victim->BeginTemporaryState(VAMPIRISM, 5000 + RAND_N(2500));

      // HP recieved is about half the damage done; against werewolves this is full
      int DrainDamage = (DoneDamage >> 1) + 1;
      if(Victim->StateIsActivated(LYCANTHROPY))
        DrainDamage = DoneDamage + 1;

    if(IsPlayer())
      game::DoEvilDeed(10);

    return Victim->ReceiveBodyPartDamage(this, DrainDamage, DRAIN, BodyPartIndex, Direction);
  }
  else
    return false;
}

truth humanoid::SpecialBiteEffect(character* Victim, v2 HitPos, int BodyPartIndex, int Direction, truth BlockedByArmour, truth Critical, int DoneDamage)
{
  if(StateIsActivated(VAMPIRISM))
  {
    if(!BlockedByArmour && Victim->IsWarmBlooded() && (!(RAND() % 2) || Critical) && !Victim->AllowSpoil())
    {
      if(IsPlayer())
        ADD_MESSAGE("You drain some precious lifeblood from %s!", Victim->CHAR_DESCRIPTION(DEFINITE));
      else if(Victim->IsPlayer() || Victim->CanBeSeenByPlayer() || CanBeSeenByPlayer())
        ADD_MESSAGE("%s drains some precious lifeblood from %s!", CHAR_DESCRIPTION(DEFINITE), Victim->CHAR_DESCRIPTION(DEFINITE));

      if(Victim->IsHumanoid() && !Victim->StateIsActivated(VAMPIRISM) && !Victim->StateIsActivated(LYCANTHROPY) && !Victim->StateIsActivated(DISEASE_IMMUNITY))
        Victim->BeginTemporaryState(VAMPIRISM, 2000 + RAND_N(500));

      // HP recieved is about half the damage done; against werewolves this is full
      int DrainDamage = (DoneDamage >> 1) + 1;
      if(Victim->StateIsActivated(LYCANTHROPY))
        DrainDamage = DoneDamage + 1;

      // To perpetuate vampirism, simply keep doing drain attacks
      BeginTemporaryState(VAMPIRISM, 50*DrainDamage);
      if(IsPlayer())
        game::DoEvilDeed(10);

      return Victim->ReceiveBodyPartDamage(this, DrainDamage, DRAIN, BodyPartIndex, Direction);
    }
    else
      return false;
  }
  else
    return false;
}

void FixSumoWrestlerHouse(festring fsCmdParams)
{
  sumowrestler* SM = NULL;
  characteridmap map = game::GetCharacterIDMapCopy();
  for(characteridmap::iterator itr = map.begin();itr!=map.end();itr++){
    character* C = itr->second;
    if(dynamic_cast<sumowrestler*>(C)){
      SM=(sumowrestler*)C;
      break;
    }
  }

  if(SM){
    for(int d = 0; d < SM->GetNeighbourSquares(); ++d)
    {
      lsquare* Square = SM->GetNeighbourLSquare(d);

      if(Square){
        character* C2 = Square->GetCharacter();
        if(C2 && dynamic_cast<bananagrower*>(C2)){
          C2->TeleportRandomly(true);
        }
      }
    }
  }
}

void sumowrestler::GetAICommand()
{
  static bool bInitDummy = [](){
    devcons::AddDevCmd("FixSumoHouse",FixSumoWrestlerHouse,
      "BugFix sumo wrestler house in case banana growers over crowd it.");
    return true;}();

  EditNP(-25);

  SeekLeader(GetLeader());

  if(CheckForEnemies(true, true, true))
    return;

  if(CheckForUsefulItemsOnGround())
    return;

  if(CheckForFood(4))
    return;

  if(FollowLeader(GetLeader()))
    return;

  if(CheckForDoors())
    return;

  if(MoveTowardsHomePos())
    return;

  EditAP(-1000);
}

void sumowrestler::BeTalkedTo()
{
  static long Said;

  if(GetRelation(PLAYER) == HOSTILE)
    ProcessAndAddMessage(GetHostileReplies()[RandomizeReply(Said, GetHostileReplies().Size)]);
  else if(!game::TweraifIsFree())
    ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, 6)]);
  else
    ProcessAndAddMessage(GetFriendlyReplies()[6 + RandomizeReply(Said, 3)]);
}

character* tourist::GetLeader() const
{
  character* Guide = game::GetTeam(TOURIST_GUIDE_TEAM)->GetLeader();
  return Guide && Guide->GetRelation(this) != HOSTILE ? Guide : GetTeam()->GetLeader();
}

void elder::GetAICommand()
{
  /* Select a place to guide the tourists to */

  if(!(RAND() % 10))
    SetGoingTo(GetLevel()->GetRandomSquare());

  humanoid::GetAICommand();
}

void tourist::GetAICommand()
{
  if(game::IsSumoWrestling() && !(RAND() % 10))
  {
    if(GetConfig() == HUSBAND)
    {
      if(RAND() & 1)
        ADD_MESSAGE("%s shouts: \"Show that skinny wimp what you've got, Huang!\"", CHAR_DESCRIPTION(DEFINITE));
      else
        ADD_MESSAGE("%s screams: \"Go for it, Huang!\"", CHAR_DESCRIPTION(DEFINITE));
    }
    else if(GetConfig() == WIFE)
    {
      if(RAND() & 1)
        ADD_MESSAGE("%s encourages you: \"Knock him out, %s!\"",
                    CHAR_DESCRIPTION(DEFINITE), game::GetPlayerName().CStr());
      else
        ADD_MESSAGE("%s cheers you: \"A handsome guy like you can't lose to that banana ball!\"",
                    CHAR_DESCRIPTION(DEFINITE));
    }
    else if(GetConfig() == CHILD)
    {
      if(RAND() & 1)
        ADD_MESSAGE("%s yells: \"More blood on the ring!!!\"", CHAR_DESCRIPTION(DEFINITE));
      else
        ADD_MESSAGE("%s cries: \"Kill him, Pong!!!\"", CHAR_DESCRIPTION(DEFINITE));
    }
  }

  humanoid::GetAICommand();
}

void imperialist::BeTalkedTo()
{
  if(GetConfig() == VICE_ROY && GetPos().IsAdjacent(PLAYER->GetPos()))
  {
    if(GetRelation(PLAYER) != HOSTILE)
    {
      decosadshirt* Shirt = static_cast<decosadshirt*>(PLAYER->SearchForItem(this, &item::IsDecosAdShirt));

      if(Shirt)
      {
        ulong Reward = Shirt->GetEquippedTicks() / 500;

        if(Reward)
        {
          ADD_MESSAGE("%s smiles. \"I see you have advertised our company diligently. "
                      "Here's %ldgp as a token of my gratitude.\"", CHAR_NAME(DEFINITE), Reward);
          PLAYER->EditMoney(Reward);
          Shirt->SetEquippedTicks(0);
          return;
        }
        else if(!(RAND() % 5))
        {
          ADD_MESSAGE("\"Come back when you've worn the shirt for some time and I'll reward you generously!\"");
          return;
        }
      }
    }

    static long Said;

    if(GetRelation(PLAYER) == HOSTILE)
      ProcessAndAddMessage(GetHostileReplies()[RandomizeReply(Said, GetHostileReplies().Size)]);
    else if(!game::PlayerIsSumoChampion())
      ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, GetFriendlyReplies().Size)]);
    else
      ProcessAndAddMessage(GetFriendlyReplies()[RandomizeReply(Said, GetFriendlyReplies().Size - 1)]);
  }
  else if(GetConfig() == HOARD_MASTER && (PLAYER->GetMoney() >= 50000) &&
          game::TweraifIsFree() && !(GetRelation(PLAYER) == HOSTILE) &&
          GetPos().IsAdjacent(PLAYER->GetPos())
         )
  {
    if(game::TruthQuestion(CONST_S("Do you want to bribe the hoardmaster? [y/n]"), REQUIRES_ANSWER))
    {
      game::TextScreen(CONST_S("\"Hmm, so you're saying you 'freed' New Attnam, right? And when you say\n"
                               "'freed', you mean 'slaughtered our soldiers and the viceroy'. Hmm...\"\n"
                               "\n"
                               "\"And what makes you think I won't call the guards this very instant, hmm?\n"
                               "I could hand you over to the master torturer. Did you know he and\n"
                               "the late viceroy were brothers? Not especially close brothers, but I think\n"
                               "he'll still want to roast you very slowly over a firepit; flay and cut and\n"
                               "tear and break you; and then have the priests heal you up to start over again.\n"
                               "So why did you come to tell me this, hmm?\""));

      game::TextScreen(CONST_S("\"Fifty thousand gold?! Oh, sorry I cried out. Hmm, yes, now I can see your point.\"\n"
                               "\n"
                               "\"But you do realize that the high priest will just send a war ship or two\n"
                               "in the Spring, and your precious New Attnam will have a new viceroy and\n"
                               "a new squad of soldiers? Even if there were no plantations, we cannot have\n"
                               "bits of the Empire just breaking free. It sets a bad example.\"\n"
                               "\n"
                               "\"Hmm, so fifty thousand gold pieces a year. And of course, you will\n"
                               "provide a steady supply of bananas. Hmm...\""));

      game::TextScreen(CONST_S("\"Hmm...\""));

      game::TextScreen(CONST_S("\"Very well! In that case, trouble yourself not with the master torturer,\n"
                               "nor with the high priest. I will handle things here, as long as you handle\n"
                               "things in your village. We have a deal. Hmm...\"\n"
                               "\n"
                               "\"Congratulations, mister. It's nice to meet the new viceroy of Tweraif.\""));

      game::PlayVictoryMusic();
      game::TextScreen(CONST_S("You are victorious!"));

      game::GetCurrentArea()->SendNewDrawRequest();
      game::DrawEverything();
      PLAYER->ShowAdventureInfo();
      festring Msg = CONST_S("became the new viceroy of Tweraif and worked hard for the well-being of his people");
      AddScoreEntry(Msg, 2, false);
      game::End(Msg);
      return;
    }
  }
  else
    humanoid::BeTalkedTo();
}

character* humanoid::CreateZombie() const
{
  if(!TorsoIsAlive())
    return 0;

  humanoid* Zombie = zombie::Spawn();
  int c;

  for(c = 0; c < BodyParts; ++c)
  {
    bodypart* BodyPart = GetBodyPart(c);

    if(!BodyPart)
    {
      BodyPart = SearchForOriginalBodyPart(c);

      if(BodyPart)
      {
        BodyPart->RemoveFromSlot();
        BodyPart->SendToHell();
      }
    }

    if(BodyPart)
    {
      bodypart* ZombieBodyPart = Zombie->GetBodyPart(c);

      if(!ZombieBodyPart)
        ZombieBodyPart = Zombie->CreateBodyPart(c);

      material* M = BodyPart->GetMainMaterial()->Duplicate();
      M->SetSpoilCounter(2000 + RAND() % 1000);
      M->SetSkinColor(Zombie->GetSkinColor());
      delete ZombieBodyPart->SetMainMaterial(M);
      ZombieBodyPart->CopyAttributes(BodyPart);
    }
    else if(!Zombie->BodyPartIsVital(c))
    {
      bodypart* ZombieBodyPart = Zombie->GetBodyPart(c);

      if(ZombieBodyPart)
      {
        ZombieBodyPart->RemoveFromSlot();
        ZombieBodyPart->SendToHell();
      }
    }
  }

  for(c = 0; c < Zombie->AllowedWeaponSkillCategories; ++c)
    Zombie->CWeaponSkill[c] = CWeaponSkill[c];

  Zombie->SWeaponSkill.resize(SWeaponSkill.size());
  std::list<sweaponskill*>::iterator i = Zombie->SWeaponSkill.begin();

  for(sweaponskill* p2 : SWeaponSkill)
    *i++ = new sweaponskill(*p2);

  memcpy(Zombie->BaseExperience,
         BaseExperience,
         BASE_ATTRIBUTES * sizeof(*BaseExperience));
  Zombie->CalculateAll();
  Zombie->RestoreHP();
  Zombie->RestoreStamina();
  static_cast<zombie*>(Zombie)->SetDescription(GetZombieDescription());
  Zombie->GenerationDanger = GenerationDanger;
  return Zombie;
}

void zombie::AddPostFix(festring& String, int Case) const
{
  if(!Description.IsEmpty())
    String << Description;
  else
    humanoid::AddPostFix(String, Case);
}

void zombie::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << Description;
}

void zombie::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> Description;
}

int darkknight::ModifyBodyPartHitPreference(int I, int Modifier) const
{
  return IsLimbIndex(I) ? Modifier << 1 : Modifier;
}

int darkknight::ModifyBodyPartToHitChance(int I, int Chance) const
{
  return IsLimbIndex(I) ? Chance << 1 : Chance;
}

void darkknight::SpecialBodyPartSeverReaction()
{
  if(!IsPlayer())
  {
    if(!(GetConfig() == MASTER))
    {
      if(IsUsingHead())
        ADD_MESSAGE("%s screams: \"I'll do you for that! I'll bite your legs off!\"", CHAR_DESCRIPTION(DEFINITE));
      else if(!(RAND() % 5))
        switch(RAND() % 3)
        {
         case 0:
          ADD_MESSAGE("%s states calmly: \"'Tis but a scratch.\"", CHAR_DESCRIPTION(DEFINITE)); break;
         case 1:
          ADD_MESSAGE("%s states calmly: \"Just a flesh wound.\"", CHAR_DESCRIPTION(DEFINITE)); break;
         case 2:
          ADD_MESSAGE("%s shouts: \"I'm invincible!\"", CHAR_DESCRIPTION(DEFINITE)); break;
        }
    }
    else if((GetConfig() == MASTER) && HasHead())
    {
      character* Called = 0;
      Called = darkknight::Spawn(ELITE);
      Called->SetTeam(GetTeam());
      Called->PutNear(GetPos());
      Called->SignalGeneration();

      if(CanBeSeenByPlayer())
      {
        ADD_MESSAGE("%s screams a profane incantation to Infuscor before disappearing.", CHAR_NAME(DEFINITE));
      }

      if(Called->CanBeSeenByPlayer())
        ADD_MESSAGE("The whole area trembles terribly as %s emerges from the shadows.", Called->CHAR_NAME(INDEFINITE));
      }
      else
        ADD_MESSAGE("You feel the sudden presence of a violent enemy nearby.");

      TeleportRandomly(true);
  }
}

void humanoid::LeprosyHandler()
{
  if(IsImmuneToLeprosy())
  {
    return;
  }

  if(!RAND_N(1000 * GetAttribute(ENDURANCE)))
    DropRandomNonVitalBodypart();

  if(!game::IsInWilderness())
  {
    for(int d = 0; d < GetNeighbourSquares(); ++d)
    {
      lsquare* Square = GetNeighbourLSquare(d);

      if(Square && Square->GetCharacter())
        Square->GetCharacter()->TryToInfectWithLeprosy(this);
    }
  }

  character::LeprosyHandler();
}

void humanoid::DropRandomNonVitalBodypart()
{
  int BodyPartIndexToDrop = GetRandomNonVitalBodyPart();

  if(BodyPartIndexToDrop != NONE_INDEX)
    DropBodyPart(BodyPartIndexToDrop);
}

void humanoid::DropBodyPart(int Index)
{
  if(!GetBodyPart(Index)->IsAlive())
    return;

  festring NameOfDropped = GetBodyPart(Index)->GetBodyPartName();
  item* Dropped = SevereBodyPart(Index);

  if(Dropped)
  {
    GetStack()->AddItem(Dropped);
    Dropped->DropEquipment();

    if(IsPlayer())
    {
      ADD_MESSAGE("You feel very ill. Your %s snaps off.", NameOfDropped.CStr());
      game::AskForKeyPress(CONST_S("Bodypart severed! [press any key to continue]"));
      DeActivateVoluntaryAction();
    }
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("Suddenly %s's %s snaps off.", CHAR_NAME(DEFINITE), NameOfDropped.CStr());
  }
  else
  {
    if(IsPlayer())
    {
      ADD_MESSAGE("You feel very ill. Your %s disappears.", NameOfDropped.CStr());
      game::AskForKeyPress(CONST_S("Bodypart destroyed! [press any key to continue]"));
      DeActivateVoluntaryAction();
    }
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("Suddenly %s's %s disappears.", CHAR_NAME(DEFINITE), NameOfDropped.CStr());
  }
}

void humanoid::DuplicateEquipment(character* Receiver, ulong Flags)
{
  character::DuplicateEquipment(Receiver, Flags);
  EnsureCurrentSWeaponSkillIsCorrect(CurrentRightSWeaponSkill, GetRightWielded());
  EnsureCurrentSWeaponSkillIsCorrect(CurrentLeftSWeaponSkill, GetLeftWielded());
}

truth character::CanHear() const
{
  return DataBase->CanHear && HasHead();
}

col16 veterankamikazedwarf::GetTorsoMainColor() const
{
  return GetMasterGod()->GetEliteColor();
}

col16 veterankamikazedwarf::GetGauntletColor() const
{
  return GetMasterGod()->GetEliteColor();
}

col16 veterankamikazedwarf::GetLegMainColor() const
{
  return GetMasterGod()->GetEliteColor();
}

col16 archangel::GetTorsoMainColor() const
{
  return GetMasterGod()->GetEliteColor();
}

col16 archangel::GetArmMainColor() const
{
  return GetMasterGod()->GetEliteColor();
}

void archangel::CreateInitialEquipment(int SpecialFlags)
{
  humanoid::CreateInitialEquipment(SpecialFlags);
  GetStack()->AddItem(holybook::Spawn(GetConfig(), SpecialFlags));

  switch(GetMasterGod()->GetBasicAlignment())
  {
   case GOOD:
    GetRightArm()->SetDexterity(70);
    GetLeftArm()->SetDexterity(70);
    break;
   case NEUTRAL:
    SetEndurance(70);
    break;
   case EVIL:
    GetRightArm()->SetStrength(70);
    GetLeftArm()->SetStrength(70);
  }
}

void zombie::PostConstruct()
{
  if(!RAND_N(3))
    GainIntrinsic(LEPROSY);
}

truth orc::MoveRandomly()
{
  return GetConfig() == REPRESENTATIVE ? MoveRandomlyInRoom() : humanoid::MoveRandomly();
}

void orc::PostConstruct()
{
  if(!RAND_N(25))
    GainIntrinsic(LEPROSY);
}

truth mistress::AllowEquipment(citem* Item, int EquipmentIndex) const
{
  return ((EquipmentIndex != RIGHT_WIELDED_INDEX
           && EquipmentIndex != LEFT_WIELDED_INDEX)
          || Item->IsWhip());
}

int humanoid::GetAttributeAverage() const
{
  return GetSumOfAttributes() / 9;
}

void golem::CreateCorpse(lsquare* Square)
{
  material* Material = GetTorso()->GetMainMaterial();

  if(Material->IsLiquid())
  {
    for(int d = 0; d < GetExtendedNeighbourSquares(); ++d)
    {
      lsquare* NeighbourSquare = Square->GetNeighbourLSquare(d);

      if(NeighbourSquare)
        NeighbourSquare->SpillFluid(0, static_cast<liquid*>(GetTorso()->GetMainMaterial()->SpawnMore(250 + RAND() % 250)));
    }
  }
  else if(Material->IsGaseous())
  {
    game::GetCurrentLevel()->GasExplosion(static_cast<gas*>(Material), Square, this);
  }
  else if(Material->IsSolid())
  {
    Square->AddItem(Material->CreateNaturalForm(ItemVolume));
  }

  SendToHell();
}

golem::golem()
{
  if(!game::IsLoading())
    ItemVolume = 50 + RAND_N(100);
}

void golem::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << ItemVolume;
}

void golem::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> ItemVolume;
}

truth humanoid::CanVomit() const
{
  return HasHead() && character::CanVomit();
}

truth humanoid::CheckApply() const
{
  if(!character::CheckApply())
    return false;

  if(!HasAUsableArm())
  {
    ADD_MESSAGE("You need a usable arm to apply.");
    return false;
  }

  return true;
}

int darkmage::GetSpellAPCost() const
{
  switch(GetConfig())
  {
   case APPRENTICE: return 4000;
   case BATTLE_MAGE: return 2000;
   case ELDER: return 1000;
   case ARCH_MAGE: return 500;
  }

  return 4000;
}

int aslonawizard::GetSpellAPCost() const
{
  /*switch(GetConfig())
  {
   case APPRENTICE: return 4000;
   case BATTLE_MAGE: return 2000;
   case ELDER: return 1000;
   case ARCH_MAGE: return 500;
 }*/

  return 1000;
}

int necromancer::GetSpellAPCost() const
{
  switch(GetConfig())
  {
   case APPRENTICE_NECROMANCER: return 2000;
   case MASTER_NECROMANCER: return 1000;
  }

  return 4000;
}

/* Horrible repeating. Sorry */

void tailor::BeTalkedTo()
{
  if(!GetPos().IsAdjacent(PLAYER->GetPos()))
    return;

  if(GetRelation(PLAYER) == HOSTILE)
  {
    ADD_MESSAGE("\"You talkin' to me? You talkin' to me? You talkin' to me? Then who "
                "the hell else are you talkin' to? You talkin' to me? Well I'm the "
                "only one here. Who do you think you're talking to? Oh yeah? Huh? Ok.\"");
    return;
  }

  if(PLAYER->PossessesItem(&item::IsFixableByTailor))
  {
    item* Item = PLAYER->SelectFromPossessions(CONST_S("\"What do you want me to fix?\""), &item::IsFixableByTailor);

    if(!Item)
      return;

    if(!(Item->GetMainMaterial()->GetCategoryFlags() & CAN_BE_TAILORED))
    {
      ADD_MESSAGE("\"I can't work on %s.\"", Item->GetMainMaterial()->GetName(false, false).CStr());
      return;
    }

    if(Item->GetMainMaterial()->IsBurning())
    {
      ADD_MESSAGE("\"Hey I'm no fire fighter! Put those flames out and then I might be able to do something for you.\"");
      return;
    }

    /** update messages */

    long FixPrice = Item->GetFixPrice();

    if(PLAYER->GetMoney() < FixPrice)
    {
      ADD_MESSAGE("\"Getting that fixed costs you %ld gold pieces. Get the money and we'll talk.\"", FixPrice);
      return;
    }

    ADD_MESSAGE("\"I can fix your %s, but it'll cost you %ld gold pieces.\"", Item->CHAR_NAME(UNARTICLED), FixPrice);

    if(game::TruthQuestion(CONST_S("Do you accept this deal? [y/N]")))
    {
      Item->RemoveBurns();
      Item->ResetThermalEnergies(); // OK because the item shouldn't be burning in the first place
      Item->ResetBurning();
      Item->Fix();
      PLAYER->EditMoney(-FixPrice);
      ADD_MESSAGE("%s fixes %s in no time.", CHAR_NAME(DEFINITE), Item->CHAR_NAME(DEFINITE));
    }
  }
  else
    ADD_MESSAGE("\"Come back when you have some weapons or armor I can fix.\"");
}

void veterankamikazedwarf::PostConstruct()
{
  kamikazedwarf::PostConstruct();

  ivantime Time;
  game::GetTime(Time);
  int Modifier = Time.Day - KAMIKAZE_INVISIBILITY_DAY_MIN;

  if(Time.Day >= KAMIKAZE_INVISIBILITY_DAY_MAX
     || (Modifier > 0
         && RAND_N(KAMIKAZE_INVISIBILITY_DAY_MAX - KAMIKAZE_INVISIBILITY_DAY_MIN) < Modifier))
    GainIntrinsic(INVISIBLE);
}

void imp::PostConstruct()
{
  humanoid::PostConstruct();

  ivantime Time;
  game::GetTime(Time);
  int Modifier = Time.Day - KAMIKAZE_INVISIBILITY_DAY_MIN;

  if(Time.Day >= KAMIKAZE_INVISIBILITY_DAY_MAX
     || (Modifier > 0
         && RAND_N(KAMIKAZE_INVISIBILITY_DAY_MAX - KAMIKAZE_INVISIBILITY_DAY_MIN) < Modifier))
    GainIntrinsic(INVISIBLE);
}

void siren::PostConstruct()
{
  humanoid::PostConstruct();

  if(GetConfig() != AMBASSADOR_SIREN)
  {
    ivantime Time;
    game::GetTime(Time);
    int Modifier = Time.Day - KAMIKAZE_INVISIBILITY_DAY_MIN;

    if(Time.Day >= KAMIKAZE_INVISIBILITY_DAY_MAX
       || (Modifier > 0
           && RAND_N(KAMIKAZE_INVISIBILITY_DAY_MAX - KAMIKAZE_INVISIBILITY_DAY_MIN) < Modifier))
      GainIntrinsic(INVISIBLE);
  }
}

truth humanoid::IsTransparent() const
{
  return character::IsTransparent() || !(GetRightLeg() || GetLeftLeg());
}

void humanoid::ModifySituationDanger(double& Danger) const
{
  character::ModifySituationDanger(Danger);

  switch(GetUsableArms())
  {
   case 0: Danger *= 10;
   case 1: Danger *= 2;
  }

  switch(GetUsableLegs())
  {
   case 0: Danger *= 10;
   case 1: Danger *= 2;
  }
}

void oree::GetAICommand()
{
  if(!RAND_N(50))
    CallForMonsters();

  humanoid::GetAICommand();
}

void oree::CallForMonsters()
{
  if(GetDungeon()->GetIndex() != ELPURI_CAVE || GetLevel()->GetIndex() != OREE_LAIR)
    return;

  character* ToBeCalled = 0;

  switch(RAND_N(6))
  {
   case 0:
    ToBeCalled = darkknight::Spawn(ELITE);
    break;
   case 1:
    ToBeCalled = frog::Spawn(RAND_2 ? GREATER_DARK : GIANT_DARK);
    break;
   case 2:
    ToBeCalled = frog::Spawn(DARK);
    break;
   case 3:
    ToBeCalled = darkmage::Spawn(RAND_2 ? APPRENTICE : BATTLE_MAGE);
    break;
   case 4:
    ToBeCalled = darkmage::Spawn(RAND_2 ? APPRENTICE : ELDER);
    break;
   case 5:
    ToBeCalled = necromancer::Spawn(RAND_2 ? APPRENTICE_NECROMANCER : MASTER_NECROMANCER);
    break;
  }

  v2 TryToCreate;

  for(int c = 0; c < 100; ++c)
  {
    TryToCreate = game::GetMonsterPortal()->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

    if(GetArea()->IsValidPos(TryToCreate)
       && ToBeCalled->CanMoveOn(GetNearLSquare(TryToCreate))
       && ToBeCalled->IsFreeForMe(GetNearLSquare(TryToCreate)))
    {
      ToBeCalled->SetTeam(game::GetTeam(MONSTER_TEAM));
      ToBeCalled->PutTo(TryToCreate);
      return;
    }
  }

  delete ToBeCalled;
}

void priest::GetAICommand()
{
  if(GetConfig() == INFUSCOR)
  {
    if(!RAND_N(50))
      CallForMonsters();
  }

  if(CheckAIZapOpportunity())
    return;

  StandIdleAI();
}

void priest::CallForMonsters()
{
  if(GetDungeon()->GetIndex() != XINROCH_TOMB || GetLevel()->GetIndex() != NECRO_CHAMBER_LEVEL)
    return;

  character* ToBeCalled = 0;

  switch(RAND_N(6))
  {
   case 0:
    ToBeCalled = skeleton::Spawn(RAND_2 ? 0 : WARRIOR);
    break;
   case 1:
    ToBeCalled = zombie::Spawn();
    break;
   case 2:
    ToBeCalled = frog::Spawn(DARK);
    break;
   case 3:
    ToBeCalled = skeleton::Spawn(RAND_2 ? 0 : WARRIOR);
    break;
   case 4:
    ToBeCalled = zombie::Spawn();
    break;
   case 5:
    ToBeCalled = necromancer::Spawn(RAND_2 ? APPRENTICE_NECROMANCER : MASTER_NECROMANCER);
    break;
  }

  v2 TryToCreate;

  for(int c = 0; c < 100; ++c)
  {
    TryToCreate = game::GetMonsterPortal()->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

    if(GetArea()->IsValidPos(TryToCreate)
       && ToBeCalled->CanMoveOn(GetNearLSquare(TryToCreate))
       && ToBeCalled->IsFreeForMe(GetNearLSquare(TryToCreate)))
    {
      ToBeCalled->SetTeam(game::GetTeam(MONSTER_TEAM));
      ToBeCalled->PutTo(TryToCreate);
      return;
    }
  }

  delete ToBeCalled;
}

int humanoid::RandomizeTryToUnStickBodyPart(ulong PossibleBodyParts) const
{
  int Possible = 0, PossibleArray[3];

  if(RightArmIsUsable() && 1 << RIGHT_ARM_INDEX & PossibleBodyParts)
    PossibleArray[Possible++] = RIGHT_ARM_INDEX;

  if(LeftArmIsUsable() && 1 << LEFT_ARM_INDEX & PossibleBodyParts)
    PossibleArray[Possible++] = LEFT_ARM_INDEX;

  if(Possible)
    return PossibleArray[RAND_N(Possible)];

  if(RightLegIsUsable() && 1 << RIGHT_LEG_INDEX & PossibleBodyParts)
    PossibleArray[Possible++] = RIGHT_LEG_INDEX;

  if(LeftLegIsUsable() && 1 << LEFT_LEG_INDEX & PossibleBodyParts)
    PossibleArray[Possible++] = LEFT_LEG_INDEX;

  if(Possible)
    return PossibleArray[RAND_N(Possible)];

  if(GetHead() && 1 << HEAD_INDEX & PossibleBodyParts)
    return HEAD_INDEX;

  if(GetGroin() && 1 << GROIN_INDEX & PossibleBodyParts)
    PossibleArray[Possible++] = GROIN_INDEX;

  if(1 << TORSO_INDEX & PossibleBodyParts)
    PossibleArray[Possible++] = TORSO_INDEX;

  return Possible ? PossibleArray[RAND_N(Possible)] : NONE_INDEX;
}

truth humanoid::HasAUsableArm() const
{
  arm* R = GetRightArm(), * L = GetLeftArm();
  return (R && R->IsUsable()) || (L && L->IsUsable());
}

truth humanoid::HasAUsableLeg() const
{
  leg* R = GetRightLeg(), * L = GetLeftLeg();
  return (R && R->IsUsable()) || (L && L->IsUsable());
}

truth humanoid::HasTwoUsableLegs() const
{
  leg* R = GetRightLeg(), * L = GetLeftLeg();
  return R && R->IsUsable() && L && L->IsUsable();
}

truth humanoid::CanAttackWithAnArm() const
{
  arm* R = GetRightArm();

  if(R && R->GetDamage())
    return true;

  arm* L = GetLeftArm();
  return L && L->GetDamage();
}

truth humanoid::RightArmIsUsable() const
{
  arm* A = GetRightArm();
  return A && A->IsUsable();
}

truth humanoid::LeftArmIsUsable() const
{
  arm* A = GetLeftArm();
  return A && A->IsUsable();
}

truth humanoid::RightLegIsUsable() const
{
  leg* L = GetRightLeg();
  return L && L->IsUsable();
}

truth humanoid::LeftLegIsUsable() const
{
  leg* L = GetLeftLeg();
  return L && L->IsUsable();
}

truth humanoid::AllowUnconsciousness() const
{
  return (DataBase->AllowUnconsciousness && TorsoIsAlive()
          && BodyPartIsVital(HEAD_INDEX));
}

truth humanoid::CanChokeOnWeb(web* Web) const
{
  return CanChoke() && Web->IsStuckToBodyPart(HEAD_INDEX);
}

truth humanoid::BrainsHurt() const
{
  head* Head = GetHead();
  return !Head || Head->IsBadlyHurt();
}

truth humanoid::IsHeadless() const
{
  head* Head = GetHead();
  return !Head;
}

void playerkind::PostConstruct()
{
  int R = 0, G = 0, B = 0;

  switch(RAND_N(4))
  {
   case 0: R = 195; G = 165; B = 40; break;
   case 1: R = 130; G = 30; B = 0; break;
   case 2: R = 30; G = 30; B = 15; break;
   case 3: R = 50; G = 30; B = 5; break;
  }

  HairColor = MakeRGB16(R + RAND_N(41), G + RAND_N(41), B + RAND_N(41));

  switch(RAND_N(4))
  {
   case 0: R = 25; G = 0; B = 70; break;
   case 1: R = 5; G = 0; B = 50; break;
   case 2: R = 10; G = 10; B = 10; break;
   case 3: R = 60; G = 20; B = 0; break;
  }

  EyeColor = MakeRGB16(R + RAND_N(41), G + RAND_N(41), B + RAND_N(41));
  Talent = RAND_N(TALENTS);
  Weakness = RAND_N(TALENTS);
}

v2 playerkind::GetHeadBitmapPos() const
{
  int Sum = GetAttribute(INTELLIGENCE, false) + GetAttribute(WISDOM, false);

  if(Sum >= 60)
    return v2(96, 480);
  else if(Sum >= 40)
    return v2(96, 464);
  else
    return v2(96, 416);
}

v2 playerkind::GetRightArmBitmapPos() const
{
  if(GetRightArm()->GetAttribute(ARM_STRENGTH, false) >= 20)
    return v2(64, 448);
  else
    return v2(64, 416);
}

v2 playerkind::GetLeftArmBitmapPos() const
{
  if(GetLeftArm()->GetAttribute(ARM_STRENGTH, false) >= 20)
    return v2(64, 448);
  else
    return v2(64, 416);
}

int playerkind::GetNaturalSparkleFlags() const
{
  return GetAttribute(CHARISMA) >= 30 ? SKIN_COLOR : 0;
}

void slave::PostConstruct()
{
  Talent = TALENT_STRONG;
  Weakness = TALENT_CLEVER;
}

cint TalentOfAttribute[ATTRIBUTES] =
{
  TALENT_HEALTHY,
  TALENT_FAST_N_ACCURATE,
  TALENT_CLEVER,
  TALENT_CLEVER,
  TALENT_CLEVER,
  TALENT_CLEVER,
  TALENT_CLEVER,
  TALENT_STRONG,
  TALENT_STRONG,
  TALENT_FAST_N_ACCURATE,
  TALENT_FAST_N_ACCURATE
};
const double TalentBonusOfAttribute[ATTRIBUTES] = { 1.1, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25 };

double playerkind::GetNaturalExperience(int Identifier) const
{
  double NE = DataBase->NaturalExperience[Identifier];

  if(Talent == TalentOfAttribute[Identifier])
    NE *= TalentBonusOfAttribute[Identifier];

  if(Weakness == TalentOfAttribute[Identifier])
    NE /= TalentBonusOfAttribute[Identifier];

  return NE;
}

v2 bonesghost::GetHeadBitmapPos() const
{
  int Sum = GetAttribute(INTELLIGENCE, false) + GetAttribute(WISDOM, false);
  // Bonesghosts have their attributes lowered upon generation, hence these lowered thresholds. Should be mathematically correct.
  if(Sum >= 52)
    return v2(96, 480);
  else if(Sum >= 32)
    return v2(96, 464);
  else
    return v2(96, 416);
}

v2 bonesghost::GetRightArmBitmapPos() const
{
  if(GetRightArm()->GetAttribute(ARM_STRENGTH, false) >= 16)
    return v2(64, 448);
  else
    return v2(64, 416);
}

v2 bonesghost::GetLeftArmBitmapPos() const
{
  if(GetLeftArm()->GetAttribute(ARM_STRENGTH, false) >= 16)
    return v2(64, 448);
  else
    return v2(64, 416);
}

void bonesghost::PostConstruct()
{
  // This seems strange, but it works because the player is still alive when a bonesghost is created.
  HairColor = PLAYER->GetHairColor();
  EyeColor = PLAYER->GetEyeColor();
}

cchar* humanoid::GetRunDescriptionLine(int I) const
{
  if(!GetRunDescriptionLineOne().IsEmpty())
    return !I ? GetRunDescriptionLineOne().CStr() : GetRunDescriptionLineTwo().CStr();

  if(IsFlying())
    return !I ? "Flying" : " very fast";

  if(IsSwimming())
  {
    if(IsPlayer() && game::IsInWilderness() && game::PlayerHasBoat())
      return !I ? "Sailing" : " very fast";
    else if(!GetRightArm() && !GetLeftArm() && !GetRightLeg() && !GetLeftLeg())
      return !I ? "Floating" : " ahead fast";
    else
      return !I ? "Swimming" : " very fast";
  }

  if(!GetRightLeg() && !GetLeftLeg())
    return !I ? "Rolling" : " very fast";

  if(!GetRightLeg() || !GetLeftLeg())
    return !I ? "Hopping" : " very fast";

  return !I ? "Running" : "";
}

cchar* humanoid::GetNormalDeathMessage() const
{
  if(BodyPartIsVital(HEAD_INDEX) && (!GetHead() || GetHead()->GetHP() <= 0))
    return !RAND_2 ? "beheaded @k" : "decapitated @k";
  else if(BodyPartIsVital(GROIN_INDEX) && (!GetGroin() || GetGroin()->GetHP() <= 0))
    return "killed @bkp dirty attack below the belt";
  else
    return character::GetNormalDeathMessage();
}

void kamikazedwarf::SingRandomSong()
{
  festring Song;
  festring God = GetMasterGod()->GetName();
  festring Bodypart;

  switch(RAND_N(9))
  {
   case 0:

    switch(RAND_N(3))
    {
     case 0:
      Bodypart = "palm";
      break;

     case 1:
      Bodypart = "forehead";
      break;

     default:
      Bodypart = "tongue";
      break;
    }
    Song = festring("On the ") + Bodypart + festring(" of ") + God + " everybody fears everything";
    break;
   case 1:
    {
      cchar* Title = GetMasterGod()->GetSex() == MALE ? "King" : "Queen";
      Song = festring("Joy to the world, ") + God
             + " is come! Let all above Valpurus receive her " + Title;
      break;
    }
   case 2:
    Song = festring("Hark the herald angels sing. Glory to ") + God + "!";
    break;
   case 3:
    Song = festring("O ") + God
           + ", You are so big, So absolutely huge, Gosh, "
           "we're all really impressed down here, I can tell You.";
    break;
   case 4:
    Song = festring("Forgive us, O ") + God
           + " for this, our dreadful toadying and barefaced flattery";
    break;
   case 5:
    Song = festring("But you, ") + God
           + ", are so strong and, well, just so super fantastic. Amen.";
    break;
   case 6:
    Song = festring("O ") + God + ", please don't burn us";
    break;
   case 7:
    Song = festring("O ") + God + ", please don't grill or toast your flock";
    break;
   case 8:
    Song = festring("O ") + God + ", please don't simmer us in stock";
    break;
  }

  EditAP(-1000);

  if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s sings: \"%s\"",
                CHAR_DESCRIPTION(DEFINITE), Song.CStr());
  else
    ADD_MESSAGE("You hear someone sing: \"%s\"", Song.CStr());
}

void imperialist::DisplayStethoscopeInfo(character*) const
{
  ADD_MESSAGE("You hear coins clinking inside.");
}

void humanoid::ApplySpecialAttributeBonuses()
{
  if(GetHead())
  {
    AttributeBonus[CHARISMA] -= GetHead()->
                                CalculateScarAttributePenalty(GetAttribute(CHARISMA, false));
  }
  else
    AttributeBonus[CHARISMA] -= GetAttribute(CHARISMA, false) - 1;
}

void siren::GetAICommand()
{
  if(TryToSing())
    return;

  humanoid::GetAICommand();
}

truth siren::MoveRandomly()
{
  if(GetConfig() == AMBASSADOR_SIREN)
  {
    return MoveRandomlyInRoom();
  }
  else
  {
    return humanoid::MoveRandomly();
  }
}

truth siren::TryToSing()
{
  truth Success = false;
  for(int d = 0; d < GetNeighbourSquares(); ++d)
  {
    lsquare* Square = GetNeighbourLSquare(d);

    if(Square && Square->GetCharacter())
    {
      Success = Square->GetCharacter()->ReceiveSirenSong(this);

      if(Success)
        break;
    }
  }
  if(Success)
    EditAP(-2000);

  return Success;
}

truth humanoid::MindWormCanPenetrateSkull(mindworm*) const
{
  if(GetHelmet())
  {
    if(RAND() % 100 < GetHelmet()->GetCoverPercentile())
      return false;
  }

  return true;
}

truth humanoid::HasSadistWeapon() const
{
  arm* Right = GetRightArm(), * Left = GetLeftArm();
  return (Right && Right->HasSadistWeapon()) || (Left && Left->HasSadistWeapon());
}

truth humanoid::HasSadistAttackMode() const
{
  return HasSadistWeapon() || IsUsingLegs();
}

void petrusswife::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << GiftTotal;
}

void petrusswife::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> GiftTotal;
}

void petrusswife::BeTalkedTo()
{
  if(!GetPos().IsAdjacent(PLAYER->GetPos()))
    return;

  itemvector Item;

  // NOTE: Remember that Petrus' wife number 5 is mute.
  if(!PLAYER->SelectFromPossessions(Item,
      (GetConfig() == 5) ? CONST_S("Do you want to offer her a gift?") : CONST_S("\"Do you have something to give me?\""),
      0, &item::IsLuxuryItem)
     || Item.empty())
    humanoid::BeTalkedTo();

  int Accepted = 0;
  truth RefusedSomething = false;

  for(size_t c = 0; c < Item.size(); ++c)
    if(!MakesBurdened(GetCarriedWeight() + Item[c]->GetWeight()))
    {
      ++Accepted;
      GiftTotal += Item[c]->GetTruePrice();
      Item[c]->RemoveFromSlot();
      GetStack()->AddItem(Item[c]);
    }
    else
    {
      RefusedSomething = true;
      break;
    }

  if((GetConfig() == 5) && (Accepted || RefusedSomething))
  {
    ADD_MESSAGE("%s smiles at you.", CHAR_NAME(DEFINITE));
    return;
  }

  if(Accepted)
    ADD_MESSAGE("\"I thank you for your little gift%s.\"", Accepted == 1 ? "" : "s");

  if(RefusedSomething)
    ADD_MESSAGE("\"Unfortunately I cannot carry any more of your gifts. I'm a delicate woman, you see.\"");
}

void guard::BeTalkedTo()
{
  if(!(GetConfig() == EMISSARY) || GetRelation(PLAYER) == HOSTILE)
  {
    if(GetPos().IsAdjacent(PLAYER->GetPos()) && !(GetRelation(PLAYER) == HOSTILE))
    {
      itemvector Item;

      if(!PLAYER->SelectFromPossessions(Item, CONST_S("\"Do you have something to give me?\""), 0, &item::IsBeverage)
         || Item.empty())


      for(size_t c = 0; c < Item.size(); ++c)
      {
        Item[c]->RemoveFromSlot();
        GetStack()->AddItem(Item[c]);
      }
    }

    humanoid::BeTalkedTo();
    return;
  }

  /* We're talking to the emissary of Aslona from now on. */
  if(game::GetStoryState() == 1)
  {
    ADD_MESSAGE("%s eyes you, calculating: \"I might have work for you and I can make it worth your while.\"", CHAR_NAME(DEFINITE));

    if(game::TruthQuestion(CONST_S("Do you accept the quest? [y/N]"), REQUIRES_ANSWER))
    {
      game::TextScreen(CONST_S("\"I shouldn't be saying this so openly, but my kingdom is in dire straits and needs any\n"
                               "help it can get. High priest Petrus will not hear my pleas and I don't believe that\n"
                               "my colleagues in other lands will be more successful. Lord Regent is doing his best,\n"
                               "but his army just barely holds the rebels back.\"\n\n"
                               "\"I know you are just one man, but maybe you could help where an army couldn't. Please,\n"
                               "go to the Castle of Aslona and seek out Lord Efra Peredivall. He will know what must\n"
                               "be done to mercilessly crush the rebel scum!\""));

      game::GivePlayerBoat();
      game::LoadWorldMap();
      v2 AslonaPos = game::GetWorldMap()->GetEntryPos(0, ASLONA_CASTLE);
      game::GetWorldMap()->GetWSquare(AslonaPos)->ChangeOWTerrain(aslonacastle::Spawn());
      game::GetWorldMap()->RevealEnvironment(AslonaPos, 1);
      v2 RebelCampPos = game::GetWorldMap()->GetEntryPos(0, REBEL_CAMP);
      game::GetWorldMap()->GetWSquare(RebelCampPos)->ChangeOWTerrain(rebelcamp::Spawn());
      game::GetWorldMap()->RevealEnvironment(RebelCampPos, 0);
      game::SaveWorldMap();
      GetArea()->SendNewDrawRequest();
      ADD_MESSAGE("\"If you need to cross the sea, you can use my ship. It should be waiting at the shore.\"");
      game::SetAslonaStoryState(1);
      game::SetStoryState(2);
      return;
    }
    else
    {
      ADD_MESSAGE("%s narrows his eyes: \"I would think twice about brushing me aside, if I were you. Think about it.\"", CHAR_NAME(DEFINITE));
      return;
    }
  }

  if(game::GetAslonaStoryState() && !RAND_N(4)) // Isn't he charming?
    ADD_MESSAGE("\"You should know that I'm counting on you. My whole country is counting on you. Don't screw it up!\"");
  else
    humanoid::BeTalkedTo();
}

void xinrochghost::GetAICommand()
{
  if(GetHomeRoom())
    StandIdleAI();
  else
    humanoid::GetAICommand();
}

void xinrochghost::CreateCorpse(lsquare* Square)
{
  SendToHell();
/*This needs to be a function someday*/
  if(!game::GetCurrentLevel()->IsOnGround())
  {
    ADD_MESSAGE("Suddenly a horrible earthquake shakes the level.");
    ADD_MESSAGE("You hear an eerie scream: \"Ahh! Free at last! FREE TO BE REBORN!\"");
    int c, Tunnels = 2 + RAND() % 3;
    if(!game::GetCurrentLevel()->EarthquakesAffectTunnels())
      Tunnels = 0;

    for(c = 0; c < Tunnels; ++c)
      game::GetCurrentLevel()->AttachPos(game::GetCurrentLevel()->GetRandomSquare(0, NOT_WALKABLE|ATTACHABLE));

    int ToEmpty = 10 + RAND() % 11;

    for(c = 0; c < ToEmpty; ++c)
      for(int i = 0; i < 50; ++i)
      {
        v2 Pos = game::GetCurrentLevel()->GetRandomSquare(0, NOT_WALKABLE);
        truth Correct = false;

        for(int d = 0; d < 8; ++d)
        {
          lsquare* Square = game::GetCurrentLevel()->GetLSquare(Pos)->GetNeighbourLSquare(d);

          if(Square && Square->IsFlyable())
          {
            Correct = true;
            break;
          }
        }

        if(Correct)
        {
          game::GetCurrentLevel()->GetLSquare(Pos)->ChangeOLTerrainAndUpdateLights(0);
          break;
        }
      }

    int ToGround = 20 + RAND() % 21;

    for(c = 0; c < ToGround; ++c)
      for(int i = 0; i < 50; ++i)
      {
        v2 Pos = game::GetCurrentLevel()->GetRandomSquare(0, RAND() & 1 ? 0 : HAS_CHARACTER);

        if(Pos == ERROR_V2)
          continue;

        lsquare* Square = game::GetCurrentLevel()->GetLSquare(Pos);
        character* Char = Square->GetCharacter();

        if(Square->GetOLTerrain() || (Char && (Char->IsPlayer() || PLAYER->GetRelation(Char) != HOSTILE)))
          continue;

        int Walkables = 0;

        for(int d = 0; d < 8; ++d)
        {
          lsquare* NearSquare = game::GetCurrentLevel()->GetLSquare(Pos)->GetNeighbourLSquare(d);

          if(NearSquare && NearSquare->IsFlyable())
            ++Walkables;
        }

        if(Walkables > 6)
        {
          Square->ChangeOLTerrainAndUpdateLights(earth::Spawn());

          if(Char)
          {
            if(Char->CanBeSeenByPlayer())
              ADD_MESSAGE("%s is hit by a brick of earth falling from the roof!", Char->CHAR_NAME(DEFINITE));

            Char->ReceiveDamage(0, 20 + RAND() % 21, PHYSICAL_DAMAGE, HEAD|TORSO, 8, true);
            Char->CheckDeath(CONST_S("killed by an earthquake"), 0);
          }

          Square->KickAnyoneStandingHereAway();
          Square->GetStack()->ReceiveDamage(0, 10 + RAND() % 41, PHYSICAL_DAMAGE);
          break;
        }
      }

    // Generate a few boulders in the level

    int BoulderNumber = 10 + RAND() % 10;

    for(c = 0; c < BoulderNumber; ++c)
    {
      v2 Pos = game::GetCurrentLevel()->GetRandomSquare();
      lsquare* Square = game::GetCurrentLevel()->GetLSquare(Pos);
      character* MonsterHere = Square->GetCharacter();

      if(!Square->GetOLTerrain() && (!MonsterHere || MonsterHere->GetRelation(PLAYER) == HOSTILE))
      {
        Square->ChangeOLTerrainAndUpdateLights(boulder::Spawn(1 + (RAND() & 1)));

        if(MonsterHere)
          MonsterHere->ReceiveDamage(0, 10 + RAND() % 10, PHYSICAL_DAMAGE, HEAD|TORSO, 8, true);

        Square->GetStack()->ReceiveDamage(0, 10 + RAND() % 10, PHYSICAL_DAMAGE);
      }
    }

    // Damage to items in the level

    for(int x = 0; x < game::GetCurrentLevel()->GetXSize(); ++x)
      for(int y = 0; y < game::GetCurrentLevel()->GetYSize(); ++y)
        game::GetCurrentLevel()->GetLSquare(x, y)->ReceiveEarthQuakeDamage();
  }
}

truth darkknight::SpecialEnemySightedReaction(character*)
{
  if((GetConfig() == MASTER))
  {
    const database* WarLordDataBase;
    databasecreator<character>::FindDataBase(WarLordDataBase, &skeleton::ProtoType, WAR_LORD);
    skeleton* Skeleton;

    if(!(WarLordDataBase->Flags & HAS_BEEN_GENERATED) && !(RAND() % 5))
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s invokes a spell!", CHAR_NAME(DEFINITE));

      Skeleton = skeleton::Spawn(WAR_LORD);
      Skeleton->SetTeam(GetTeam());
      Skeleton->PutNear(GetPos());
      Skeleton->SignalGeneration();

      if(Skeleton->CanBeSeenByPlayer())
        ADD_MESSAGE("The whole area trembles terribly as %s emerges from the ground.", Skeleton->CHAR_NAME(DEFINITE));
      else if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s casts a powerful spell which makes the whole area tremble.", CHAR_NAME(DEFINITE));
      else
        ADD_MESSAGE("You feel the presence of an ancient evil being awakened from its long slumber. You shiver.");

      Skeleton->SetGenerationDanger(GetGenerationDanger());
      return true;
    }
    else
      return false;
  }
  else
    return false;
}

truth darkknight::CheckForUsefulItemsOnGround(truth CheckFood)
{
  if(GetConfig() == MASTER)
    return false;
  else
    return character::CheckForUsefulItemsOnGround(CheckFood);
}

void goblin::GetAICommand()
{
  if(CheckAIZapOpportunity())
    return;

  humanoid::GetAICommand();
}

void cossack::GetAICommand()
{
  if(CheckAIZapOpportunity())
    return;

  humanoid::GetAICommand();
}

void werewolfwolf::GetAICommand()
{
  if(GetConfig() == DRUID && !RAND_N(4))
    if(CheckAIZapOpportunity())
      return;

  humanoid::GetAICommand();
}

truth humanoid::CheckAIZapOpportunity()
{
  if(!HasAUsableArm() || StateIsActivated(CONFUSED))
    return false;
  else
    return character::CheckAIZapOpportunity();
}

truth imp::SpecialEnemySightedReaction(character* Char)
{
  if(GetPos().IsAdjacent(Char->GetPos()))
  {
    if((StateIsActivated(PANIC) || IsInBadCondition()) && !RAND_4)
    {
      if(CanBeSeenByPlayer())
        ADD_MESSAGE("%s shrieks!", CHAR_NAME(DEFINITE));

      TeleportRandomly(true);
      return true;
    }
    else
      return false;
  }

  if(GetHP() > (GetMaxHP() / 2) && !RAND_N(10))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s chortles.", CHAR_NAME(DEFINITE));

    TeleportNear(Char);
    return true;
  }

  return false;
}

truth crimsonimp::SpecialBiteEffect(character* Victim, v2 HitPos, int BodyPartIndex, int Direction,
                                    truth BlockedByArmour, truth Critical, int DoneDamage)
{
  bodypart* BodyPart = Victim->GetBodyPart(BodyPartIndex);

  if(BodyPart && BodyPart->IsDestroyable(Victim)
     && BodyPart->GetMainMaterial() && BodyPart->CanBeBurned()
     && (BodyPart->GetMainMaterial()->GetInteractionFlags() & CAN_BURN)
     && !BodyPart->IsBurning())
      {
        if(BodyPart->TestActivationEnergy(150))
        {
          return true;
        }
      }

  return false;
}

void crimsonimp::CreateCorpse(lsquare* Square)
{
  game::GetCurrentLevel()->Explosion(this, "consumed by the hellfire of "  + GetName(INDEFINITE),
                                     Square->GetPos(), 20 + RAND() % 5 - RAND() % 5);
  SendToHell();
}

truth mirrorimp::DrinkMagic(const beamdata& Beam)
{
  if(!Beam.Wand)
    return false;
  if(!Beam.Wand->IsExplosive())
    return false;

  if(Beam.Owner && RAND_N(GetAttribute(MANA)) <= RAND_N(Beam.Owner->GetAttribute(WILL_POWER)))
  {
    Beam.Owner->EditExperience(WILL_POWER, 100, 1 << 12);
    return false;
  }

  festring DeathMsg = CONST_S("killed by an explosion of ");
  Beam.Wand->AddName(DeathMsg, INDEFINITE);
  DeathMsg << " caused @bk";

  if(IsPlayer())
    ADD_MESSAGE("You grin as %s %s.", Beam.Wand->GetExtendedDescription().CStr(), Beam.Wand->GetBreakMsg().CStr());
  else if(CanBeSeenByPlayer())
    ADD_MESSAGE("%s cackles with glee as %s %s.", CHAR_NAME(DEFINITE), Beam.Wand->GetExtendedDescription().CStr(),
                                                  Beam.Wand->GetBreakMsg().CStr());

  Beam.Wand->BreakEffect(this, DeathMsg);
  return true;
}

void mirrorimp::CreateCorpse(lsquare* Square)
{
  decoration* Shard = decoration::Spawn(SHARD);
  Shard->InitMaterials(MAKE_MATERIAL(GLASS));
  Square->ChangeOLTerrainAndUpdateLights(Shard);
  SendToHell();
}

void elder::BeTalkedTo()
{
  if(game::TweraifIsFree() && !game::GetFreedomStoryState() && !HasBeenSpokenTo
     && !(GetRelation(PLAYER) == HOSTILE) && GetPos().IsAdjacent(PLAYER->GetPos()))
  {
    game::TextScreen(CONST_S("\"My boy, my wonderful boy! From the very day I found you,\n"
                             "I knew there was something special in you, something even\n"
                             "the accursed hippos couldn't spoil. And now you have saved us\n"
                             "from valpurian clutches and given us a chance at freedom!\n"
                             "Thank you so very, very much.\"\n\n"
                             "\"Alas, I'm afraid Tweraif is not yet out of the proverbial woods.\n"
                             "We are few and the Attnamese army is massive. Their battleships\n"
                             "will be ready once the winter ends and the ice thaws, and they will\n"
                             "not hesitate to bring their tyranny back. I still don't get why they\n"
                             "love those bananas so much.\"\n\n"
                             "\"We have no hope to defeat them in a fight, so fight them we shan't.\""));

    game::TextScreen(CONST_S("\"Let me tell you a story, or a myth if you will.\"\n\n"
                             "\"Once upon a time, there was a town. No one could find the town\n"
                             "unless they already knew where it was, and on one could enter\n"
                             "uninvited. The town was called Mondedr and it was concealed\n"
                             "from the world by the power of Cleptia. It was never conquered.\"\n\n"
                             "\"The thing is, I know for a fact that Mondedr exists, and that\n"
                             "their cloaking spell can be replicated. Attnam tried to take our\n"
                             "goddess away, but she is still strong in our hearts. I have faith\n"
                             "she will protect this island from valpurians, just as Cleptia did\n"
                             "for Mondedr.\""));

    game::TextScreen(CONST_S("\"The prayers are simple, but no god can affect the world uninvited,\n"
                             "and a miracle of such strength requires more power than any priest\n"
                             "could channel. We need a conduit, something close to Silva herself.\"\n\n"
                             "\"We need a scion of the Holy Mango World-tree.\"\n\n"
                             "\"You have done so much for your village, yet I must ask for another\n"
                             "favour. You know that the late viceroy destroyed the altar of Silva\n"
                             "in our shrine, but you might not know that there is another shrine of Silva\n"
                             "on this island, or rather below it. I would implore you to go down into\n"
                             "the underwater tunnel and find a strange formation of rock where our people\n"
                             "buried the stairs to the crystal cave of Silva under a cave-in,\n"
                             "once it was obvious that we will be conquered. We couldn't let the Attnamese\n"
                             "desecrate that most holy place. There, in an ancient temple of Silva,\n"
                             "grows a tree of wondrous power, a tiny sapling of the World-tree.\""));

    game::TextScreen(CONST_S("\"Please, bring back a seedling of this tree. Once we plant it here,\n"
                             "in the village, I can cast the spell and no army will find us.\n"
                             "The first valpurian attack surprised us, caught us unaware, unprepared\n"
                             "and unable to defend our land. So let's not repeat history and\n"
                             "get ready for them this time.\""));

    game::SetFreedomStoryState(1);
    GetArea()->SendNewDrawRequest();
    ADD_MESSAGE("\"Oh, and give my regards to Terra, if she's still alive.\"");

    HasBeenSpokenTo = true;
  }
  else if((game::GetFreedomStoryState() == 2) && !(GetRelation(PLAYER) == HOSTILE))
  {
    ADD_MESSAGE("\"You have the seedling! Wonderful. Please, plant it by the banana delivery spot, and we shan't fear the imperialists anymore.\"");
  }
  else
    humanoid::BeTalkedTo();
}

void terra::BeTalkedTo()
{
  if((game::GetFreedomStoryState() == 1) && !HasBeenSpokenTo && !(GetRelation(PLAYER) == HOSTILE)
     && GetPos().IsAdjacent(PLAYER->GetPos()))
  {
    game::TextScreen(CONST_S("\"Tweraif has been freed?! What wonderful news you bring me!\"\n\n"
                             "\"I have volunteered all those years ago to be buried here in this cave\n"
                             "along with the shrine, to tend it and to protect the rites and traditions\n"
                             "that the Attnamese would rather see burnt and forgotten. Yet I have hoped\n"
                             "every day that a word would come about an end to the tyranny, that\n"
                             "I would be free to return home. I guess my hope dwindled over the years,\n"
                             "but you are here now and my wishes came true. Thank you.\"\n\n"
                             "\"Nevertheless, I know what you came for. A seedling of this holy tree,\n"
                             "to channel the power of Silva and shroud Tweraif against further attacks.\n"
                             "I wish it was that simple, but I have no seeds to give you.\""));

    game::TextScreen(CONST_S("\"You see, this shrine is built in a remote, lost cave for a reason.\n"
                             "It is a guarding post, a bulwark, and a seal on a prison.\"\n\n"
                             "\"One thousand years ago, Nefas, the goddess of forbidden pleasures,\n"
                             "came to Scabies, the goddess of diseases, mutations and deformity,\n"
                             "in the form of a handsome hero, and seduced her. Whether it was\n"
                             "for Nefas' own amusement, or the humiliation Scabies suffered\n"
                             "when she discovered who she laid with, no one knows, but Scabies got\n"
                             "pregnant and eventually delivered a divine baby - a monstrous spider\n"
                             "the likes of which this world had never seen before.\"\n\n"
                             "\"The spider was a behemoth of her kind, massive and terrifying\n"
                             "and truly detestable. Spurned and abandoned by both her mothers,\n"
                             "the spider rampaged through the world until she was defeated and\n"
                             "bound by a circle of druids and priests. Her name is Lobh-se and\n"
                             "she is imprisoned below this cave, trapped by the power of Silva\n"
                             "channeled through the Holy Mango Tree.\""));

    game::TextScreen(CONST_S("\"Lobh-se is a terrible creature, an avatar of famine and consumption.\n"
                             "She breeds thousands of lesser spiders and immediately devours them\n"
                             "in her endless hunger. She strains against her bonds and even comes here,\n"
                             "feasting on the animals attracted to the magicks of Silva, and on the few\n"
                             "plants that scrape a living this deep underground. I can somewhat keep her\n"
                             "at bay, protecting myself and the tree, but the magic of the holy seedlings\n"
                             "is sweet to Lobh-se, and not strong enough to ward her off. She devoured\n"
                             "the last seedling just a few days ago.\"\n\n"
                             "\"You are a hero already for liberating our village,\n"
                             "but if you really wish to ensure the safety of Tweraif, you have to venture\n"
                             "deeper, to the very lair of Lobh-se. She may be a godling, but her body\n"
                             "is still mortal. Cut the seedling from her gullet, and I will keep her spirit\n"
                             "bound so that it cannot create a new body to harass this world.\""));

    game::TextScreen(CONST_S("\"May Silva bless you in your doings.\""));

    GetArea()->SendNewDrawRequest();
    ADD_MESSAGE("\"Oh, and give my love to Kaethos, if he's still alive.\"");

    HasBeenSpokenTo = true;
  }
  else if((game::GetFreedomStoryState() == 2) && !(GetRelation(PLAYER) == HOSTILE))
  {
    priest::BeTalkedTo(); // in case player also needs a cure, before the tip (below) to grant it wont be ignored
    ADD_MESSAGE("\"You bested her, I see! Now hurry back to the village, and Attnam shall threaten us no more.\"");
  }
  else
    priest::BeTalkedTo();
}

void aslonawizard::GetAICommand()
{
  SeekLeader(GetLeader());

  if(FollowLeader(GetLeader()))
    return;

  /*
   * Teleports when in danger, otherwise either blinks his allies close to
   * an enemy, or summons a gas golem.
   */

  character* NearestEnemy = 0;
  long NearestEnemyDistance = 0x7FFFFFFF;
  character* RandomFriend = 0;
  charactervector Friend;
  v2 Pos = GetPos();

  for(int c = 0; c < game::GetTeams(); ++c)
  {
    if(GetTeam()->GetRelation(game::GetTeam(c)) == HOSTILE)
    {
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled())
        {
          long ThisDistance = Max<long>(abs(p->GetPos().X - Pos.X), abs(p->GetPos().Y - Pos.Y));

          if((ThisDistance < NearestEnemyDistance
              || (ThisDistance == NearestEnemyDistance && !(RAND() % 3))) && p->CanBeSeenBy(this))
          {
            NearestEnemy = p;
            NearestEnemyDistance = ThisDistance;
          }
        }
    }
    else if(GetTeam()->GetRelation(game::GetTeam(c)) == FRIEND)
    {
      for(character* p : game::GetTeam(c)->GetMember())
        if(p->IsEnabled() && p->CanBeSeenBy(this))
          Friend.push_back(p);
    }
  }

  if(NearestEnemy && NearestEnemy->GetPos().IsAdjacent(Pos) &&
     (!(RAND() % 4) || StateIsActivated(PANIC)))
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s invokes a spell and disappears.", CHAR_NAME(DEFINITE));

    TeleportRandomly(true);
    EditAP(-GetSpellAPCost());
    return;
  }

  if(!RAND_2 && CheckAIZapOpportunity())
    return;

  if(NearestEnemy && (NearestEnemyDistance < 10 || StateIsActivated(PANIC)) && RAND() & 3)
  {
    SetGoingTo((Pos << 1) - NearestEnemy->GetPos());

    if(MoveTowardsTarget(true))
      return;
  }

  if(Friend.size() && !(RAND() & 3))
  {
    RandomFriend = Friend[RAND() % Friend.size()];
    NearestEnemy = 0;
  }

  if(GetRelation(PLAYER) == HOSTILE && PLAYER->CanBeSeenBy(this))
    NearestEnemy = PLAYER;

  beamdata Beam
    (
      this,
      CONST_S("killed by the spells of ") + GetName(INDEFINITE),
      YOURSELF,
      0
    );

  if(NearestEnemy)
  {
    if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s invokes a spell!", CHAR_NAME(DEFINITE));

    if(RandomFriend && !RAND_N(4))
    {
      EditAP(-GetSpellAPCost());

      RandomFriend->GetLSquareUnder()->DrawParticles(RED);
      RandomFriend->TeleportNear(NearestEnemy);
      return;
    }
    else
    {
      lsquare* Square = NearestEnemy->GetLSquareUnder();
      character* ToBeCalled = 0;

      EditAP(-GetSpellAPCost());

      int GasMaterial[] = { MUSTARD_GAS, MAGIC_VAPOUR, SLEEPING_GAS, TELEPORT_GAS,
                            EVIL_WONDER_STAFF_VAPOUR, EVIL_WONDER_STAFF_VAPOUR };
      ToBeCalled = golem::Spawn(GasMaterial[RAND() % 6]);
      v2 Where = GetLevel()->GetNearestFreeSquare(ToBeCalled, Square->GetPos());

      if(Where == ERROR_V2)
      {
        if(CanBeSeenByPlayer())
          ADD_MESSAGE("Nothing happens.");

        delete ToBeCalled;
      }
      else
      {
        ToBeCalled->SetGenerationDanger(GetGenerationDanger());
        ToBeCalled->SetTeam(GetTeam());
        ToBeCalled->PutTo(Where);

        if(ToBeCalled->CanBeSeenByPlayer())
          ADD_MESSAGE("Suddenly %s materializes!", ToBeCalled->CHAR_NAME(INDEFINITE));

        ToBeCalled->GetLSquareUnder()->DrawParticles(RED);
      }

      if(CanBeSeenByPlayer())
        NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell of ") + GetName(DEFINITE)
                                                + CONST_S(" interrupts you."));
      else
        NearestEnemy->DeActivateVoluntaryAction(CONST_S("The spell interrupts you."));

      return;
    }
  }

  StandIdleAI();
}

int gasghoul::TakeHit(character* Enemy, item* Weapon, bodypart* EnemyBodyPart, v2 HitPos, double Damage,
                      double ToHitValue, int Success, int Type, int Direction, truth Critical, truth ForceHit)
{
  int Return = humanoid::TakeHit(Enemy, Weapon, EnemyBodyPart, HitPos, Damage,
                                 ToHitValue, Success, Type, Direction, Critical, ForceHit);

  if(Return != HAS_DODGED && Return != HAS_BLOCKED && GetLSquareUnder()->IsFlyable())
  {
    if(Enemy->IsPlayer())
      ADD_MESSAGE("%s releases a cloud of fumes as you strike %s.", CHAR_DESCRIPTION(DEFINITE), GetObjectPronoun().CStr());
    else if(IsPlayer())
      ADD_MESSAGE("You release a cloud of fumes as %s strikes you.", Enemy->CHAR_DESCRIPTION(DEFINITE));
    else if(CanBeSeenByPlayer() && Enemy->CanBeSeenByPlayer())
      ADD_MESSAGE("%s releases a cloud of fumes as %s strikes %s.", CHAR_DESCRIPTION(DEFINITE), Enemy->CHAR_DESCRIPTION(DEFINITE),
                                                                    GetObjectPronoun().CStr());
    else if(CanBeSeenByPlayer())
      ADD_MESSAGE("%s releases a cloud of fumes as something strikes %s.", CHAR_DESCRIPTION(DEFINITE), GetObjectPronoun().CStr());

    int GasMaterial[] = { MUSTARD_GAS, SKUNK_SMELL, ACID_GAS, FIRE_GAS };

    if(Critical)
      GetLevel()->GasExplosion(gas::Spawn(GasMaterial[RAND() % 4], 100), GetLSquareUnder(), this);
    else
      GetLSquareUnder()->AddSmoke(gas::Spawn(GasMaterial[RAND() % 4], 100));
  }

  return Return;
}

void elder::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << HasBeenSpokenTo;
}

void elder::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> HasBeenSpokenTo;
}

void terra::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << HasBeenSpokenTo;
}

void terra::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> HasBeenSpokenTo;
}

void aslonawizard::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << HasBeenSpokenTo;
}

void aslonawizard::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> HasBeenSpokenTo;
}

void aslonacaptain::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << HasBeenSpokenTo;
}

void aslonacaptain::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> HasBeenSpokenTo;
}

void aslonapriest::Save(outputfile& SaveFile) const
{
  humanoid::Save(SaveFile);
  SaveFile << HasBeenSpokenTo;
}

void aslonapriest::Load(inputfile& SaveFile)
{
  humanoid::Load(SaveFile);
  SaveFile >> HasBeenSpokenTo;
}

void aslonawizard::BeTalkedTo()
{
  if(GetPos().IsAdjacent(PLAYER->GetPos()) && !(GetRelation(PLAYER) == HOSTILE))
  {
    if((game::GetAslonaStoryState() > 1) && !HasBeenSpokenTo)
    {
      game::TextScreen(CONST_S("\"You have been sent by Lord Regent? Excellent! As the only protector of Aslona from\n"
                               "supernatural incursions, I cannot leave the castle, so I shall welcome any help you\n"
                               "can provide.\"\n\n"
                               "\"My request for you might cause some to brand me a coward. But no matter. We must win\n"
                               "the war quickly, or Aslona looses even if we eventually achieve victory. Swords and\n"
                               "spells may have won earlier battles, but now we need to end the rebels in one fell swoop.\n"
                               "Lord Mittrars is already working hard on locating the command centre of Harvan's forces,\n"
                               "so my task is to arrange for the weapon.\"\n\n"
                               "\"My research shows that such a weapon capable of complete obliteration appears in many\n"
                               "tales from the long lost empire of Otoul'iv Ik-Omit. I have uncovered the site of\n"
                               "a pyramid dating back to that era, and all sources indicate that deep within, an untouched\n"
                               "arms depot should be located.\"\n\n"
                               "\"Please fetch me this mighty weapon post haste. I'm sure you will recognize it once you see it.\""));

      game::LoadWorldMap();
      v2 PyramidPos = game::GetWorldMap()->GetEntryPos(0, PYRAMID);
      game::GetWorldMap()->GetWSquare(PyramidPos)->ChangeOWTerrain(pyramid::Spawn());
      game::GetWorldMap()->RevealEnvironment(PyramidPos, 1);
      game::SaveWorldMap();

      GetArea()->SendNewDrawRequest();
      ADD_MESSAGE("%s says with a concern: \"Be careful, the pyramid is said to be a dangerous place. Better be prepared when you go there.\"", CHAR_NAME(DEFINITE));

      HasBeenSpokenTo = true;
      return;
    }
    else if(PLAYER->HasNuke())
    {
      if(game::TruthQuestion(CONST_S("Turn in the thaumic bomb? [y/N]"), REQUIRES_ANSWER))
      {
        PLAYER->RemoveNuke(this);
        ADD_MESSAGE("%s beams: \"Yes! Thank you, thank you! With this, we can blow the rebels to tiny bits, we can end the war!\"", CHAR_NAME(DEFINITE));
        game::SetAslonaStoryState(game::GetAslonaStoryState() + 1);
        return;
      }
    }
  }

  humanoid::BeTalkedTo();
}

void aslonawizard::CreateCorpse(lsquare* Square)
{
  game::GetCurrentLevel()->GasExplosion(gas::Spawn(MAGIC_VAPOUR, 100), Square, this);
  SendToHell();
}

void aslonacaptain::BeTalkedTo()
{
  if(GetPos().IsAdjacent(PLAYER->GetPos()) && !(GetRelation(PLAYER) == HOSTILE))
  {
    if((game::GetAslonaStoryState() > 1) && !HasBeenSpokenTo)
    {
      game::TextScreen(CONST_S("\"Finally someone not tangled up in their responsibilities! I was almost ready to drop\n"
                               "everything and go myself, but thankfully you are here.\"\n\n"
                               "\"Let me explain. When king Othyr died, Seges rest his soul, and Harvan fled justice to\n"
                               "start the rebellion, the crown prince, His Highness Artorius, was away on a visit to Castle\n"
                               "Noth. I sent for him to immediately return back to the Castle of Aslona, both for safety\n"
                               "and to pay respects to his father. But his retinue never arrived. My scouts found marks\n"
                               "of an ambush and tracked the responsible raiding party of goblins back to their lair\n"
                               "in a nearby ruined fort, where we also believe they imprisoned the young prince.\"\n\n"
                               "\"I would have loved to throw the whole army of Aslona at the goblins, but with\n"
                               "the ceaseless attacks of rebel squads, that would mean sacrificing the kingdom.\n"
                               "I was forced to wait for a momentary respite from the fighting, or for some trustworthy\n"
                               "outsider willing to go on a rescue mission.\"\n\n"
                               "\"It tears at my heart that poor prince languishes in some jail cell, as if his father's\n"
                               "death wasn't hard on him already. Save prince Artorius, I beg of you, and bring him to me\n"
                               "so that I need to worry no more.\""));

      game::LoadWorldMap();
      v2 GoblinPos = game::GetWorldMap()->GetEntryPos(0, GOBLIN_FORT);
      game::GetWorldMap()->GetWSquare(GoblinPos)->ChangeOWTerrain(goblinfort::Spawn());
      game::GetWorldMap()->RevealEnvironment(GoblinPos, 1);
      game::SaveWorldMap();

      GetArea()->SendNewDrawRequest();
      ADD_MESSAGE("%s says: \"Hurry, please.\"", CHAR_NAME(DEFINITE));

      HasBeenSpokenTo = true;
      return;
    }
    else if(HasBeenSpokenTo)
    {
      // Does the player have prince Artorius in his team?
      character* CrownPrince = 0;
      for(character* p : game::GetTeam(PLAYER_TEAM)->GetMember())
        if(p->IsEnabled() && !p->IsPlayer() && p->IsKing())
          CrownPrince = p;

      if(CrownPrince)
      {
        if(game::TruthQuestion(CONST_S("Entrust young prince to Lord Mittrars' care? [y/N]"), REQUIRES_ANSWER))
        {
          team* Team = game::GetTeam(ASLONA_TEAM);
          CrownPrince->ChangeTeam(Team);

          ADD_MESSAGE("\"Uncle Mittrars!\"");
          ADD_MESSAGE("\"My prince! Thanks Seges and all the gods of Law, I was already loosing any hope that I will see you again.\"");
          game::SetAslonaStoryState(game::GetAslonaStoryState() + 1);
          return;
        }
      }
    }
  }

  humanoid::BeTalkedTo();
}

void aslonapriest::BeTalkedTo()
{
  if(GetPos().IsAdjacent(PLAYER->GetPos()) && !(GetRelation(PLAYER) == HOSTILE))
  {
    if((game::GetAslonaStoryState() > 1) && !HasBeenSpokenTo)
    {
      game::TextScreen(CONST_S("\"Yes, I can most definitely put your skills to good use. You see, this senseless\n"
                               "war has already claimed many lives by the blades of the soldiers, but a more insidious\n"
                               "enemy is rearing her ugly head. I'm talking about Scabies. With many injured and access\n"
                               "to supplies limited, diseases are starting to spread, and even my skills and magic\n"
                               "are not enough without proper medical care and hygiene. Hygiene is essential to health,\n"
                               "but essential to hygiene is access to clean water, which is one of the things this castle\n"
                               "lacks right now.\"\n\n"
                               "\"I have found some adventurer's journal describing an artifact that might help solve\n"
                               "our troubles. A single tear of Silva, petrified into the form of an obsidian shard, yet still\n"
                               "weeping with rains of freshwater. I would ask of you to retrieve this shard for me. It should\n"
                               "be located in a nearby coal cave, though you should know the journal mentioned strange fungal\n"
                               "growths appearing in the cave, nourished by the life-giving water.\""));

      game::LoadWorldMap();
      v2 CavePos = game::GetWorldMap()->GetEntryPos(0, FUNGAL_CAVE);
      game::GetWorldMap()->GetWSquare(CavePos)->ChangeOWTerrain(fungalcave::Spawn());
      game::GetWorldMap()->RevealEnvironment(CavePos, 1);
      game::SaveWorldMap();

      GetArea()->SendNewDrawRequest();
      ADD_MESSAGE("%s says: \"Thank you very much for your kind help.\"", CHAR_NAME(DEFINITE));

      HasBeenSpokenTo = true;
      return;
    }
    else if(PLAYER->HasWeepObsidian())
    {
      if(game::TruthQuestion(CONST_S("Turn in the weeping obsidian? [y/N]"), REQUIRES_ANSWER))
      {
        PLAYER->RemoveWeepObsidian(this);
        ADD_MESSAGE("%s says: \"Wonderful! Let me get to work right away.\"", CHAR_NAME(DEFINITE));
        game::SetAslonaStoryState(game::GetAslonaStoryState() + 1);
        return;
      }
    }
  }

  priest::BeTalkedTo();
}

void harvan::BeTalkedTo()
{
  if(GetPos().IsAdjacent(PLAYER->GetPos()) && !(GetRelation(PLAYER) == HOSTILE))
  {
    if(!game::GetRebelStoryState())
    {
      game::TextScreen(CONST_S("\"Well met, adventurer! It's always nice to talk to someone unaffected by\n"
                               "the unfortunate quarrels of our kingdom.\"\n\n"
                               "\"I don't know how much have you heard, but our old king Othyr was murdered\n"
                               "and then even the crown prince Artorius disappeared in an alleged goblin raid.\n"
                               "Immediately, Lord Efra Peredivall named himself the Lord Regent of Aslona and\n"
                               "made it known that he intends to *use* his newfound power. I have known\n"
                               "Lord Peredivall for most of my life, I used to call him my friend, but I cannot\n"
                               "stand for this high treason, and the people of Aslona support me. But now we must\n"
                               "hide in the woods like brigands, hunted by those corrupt or mislead by\n"
                               "Lord Regent's lies. I fear we will need some edge if we want to win this war\n"
                               "without drowning in blood.\""));

      game::TextScreen(CONST_S("\"Ah-hah! And here I'm talking without realizing you could be of a great help!\n"
                               "You are an obvious foreigner, hardly suspected of any connections to us. You could\n"
                               "infiltrate Lord Regent's forces and try to glean his plans. Maybe even offer\n"
                               "your services and whatever he asks of you to acquire, bring to us instead!\n"
                               "Yes, go to the Castle of Aslona, help our cause and once we achieve victory,\n"
                               "I will reward you handsomely.\""));

      GetArea()->SendNewDrawRequest();
      ADD_MESSAGE("%s pats you on the back. \"Good luck and return soon, my friend.\"", CHAR_NAME(DEFINITE));
      game::SetRebelStoryState(2); // To have same StoryState values as Aslona.
      return;
    }
    else if(PLAYER->HasMasamune())
    {
      if(game::TruthQuestion(CONST_S("Turn in the noble katana named E-numa sa-am? [y/N]"), REQUIRES_ANSWER))
      {
        game::PlayVictoryMusic();
        game::TextScreen(CONST_S("You hand over the ancient katana and thus the regalia necessary to crown a new\n"
                                 "king of Aslona are together again.\n\n"
                                 "\"Thank you, my friend,\" Harvan says and then turns to his army. \"Comrades,\n"
                                 "this is our hour of victory. You have fought well. You are heroes, all of you.\n"
                                 "Our country and the very soul of our nation was saved only thanks to you, thanks\n"
                                 "to your courage, loyalty, selflessness and resolve. But now, the war is over.\n"
                                 "The traitors shall face justice and peace will return to our homes. Gods bless Aslona!\"\n\n"
                                 "A deafening cheer echoes his words.\n\nYou are victorious!"));

        game::GetCurrentArea()->SendNewDrawRequest();
        game::DrawEverything();
        PLAYER->ShowAdventureInfo();

        // Did the player do all quests just for rebels?
        festring Msg;
        if(game::GetRebelStoryState() == 5)
        {
          Msg = CONST_S("helped the rebels to an overwhelming victory");
          AddScoreEntry(Msg, 4, false);
        }
        else
        {
          Msg = CONST_S("helped the rebels to an uneasy victory");
          AddScoreEntry(Msg, 2, false);
        }

        game::End(Msg);
        return;
      }
    }
    else if(PLAYER->HasNuke())
    {
      if(game::TruthQuestion(CONST_S("Turn in the thaumic bomb? [y/N]"), REQUIRES_ANSWER))
      {
        PLAYER->RemoveNuke(this);
        ADD_MESSAGE("\"So this is the fate Lord Regent had planned for me and my people. Thank you for saving all of our lives, %s.\"",
                    PLAYER->GetAssignedName().CStr());
        game::SetRebelStoryState(game::GetRebelStoryState() + 1);
        return;
      }
    }
    else if(PLAYER->HasWeepObsidian())
    {
      if(game::TruthQuestion(CONST_S("Turn in the weeping obsidian? [y/N]"), REQUIRES_ANSWER))
      {
        PLAYER->RemoveWeepObsidian(this);
        ADD_MESSAGE("\"The royalists are starting to feel their isolation in the castle, it would seem. Sooner or later, they will loose their strength to oppose us.\"");
        game::SetRebelStoryState(game::GetRebelStoryState() + 1);
        return;
      }
    }
    else if(game::GetRebelStoryState() == 5 && game::GetStoryState() < 3)
    {
      game::TextScreen(CONST_S("\"You are a hero to me, you should know that. You have already done so much,\n"
                               "yet I must ask for one last favor.\"\n\n"
                               "\"This war is costly in both innocent lives and money we cannot spare,\n"
                               "it needs to end right now. Thanks to your help, we have the thaumic bomb and\n"
                               "could level the whole castle and wipe Lord Regent and his royalists from the surface\n"
                               "of the world, but I'm unwilling to sacrifice everyone in the castle and the castle\n"
                               "with itself. It would be a hollow victory to kill the traitor, but loose the kingdom\n"
                               "when the symbol of our proud history was in ruins.\"\n\n"
                               "\"Yet there is a third option. It is my sincere belief that many of Lord Peredivall's\n"
                               "troops would change sides if prince Artorius took on the title of his father as\n"
                               "the rightful king of Aslona. But for His Highness to be crowned the king without\n"
                               "any doubts or dispute, we need the regalia of Aslona, two ancient, masterwork katanas,\n"
                               "Asamarum and E-numa sa-am.\""));

      game::TextScreen(CONST_S("\"I have managed to take Asamarum with me during Lord Regent's coup, but he still\n"
                               "holds E-numa sa-am. One last time, I would ask you to steal into the Castle of Aslona\n"
                               "and retrieve the sword from Lord Regent, so that we might claim victory without\n"
                               "bloodshed.\""));

      GetArea()->SendNewDrawRequest();
      PLAYER->GetTeam()->Hostility(game::GetTeam(ASLONA_TEAM)); // Too easy otherwise.
      ADD_MESSAGE("%s hugs you tightly. \"Godspeed, my friend.\"", CHAR_NAME(DEFINITE));
      game::SetStoryState(3);
      return;
    }
    else
    {
      // Does the player have prince Artorius in his team?
      character* CrownPrince = 0;
      for(character* p : game::GetTeam(PLAYER_TEAM)->GetMember())
        if(p->IsEnabled() && !p->IsPlayer() && p->IsKing())
          CrownPrince = p;

      if(CrownPrince)
      {
        if(game::TruthQuestion(CONST_S("Entrust young prince to Harvan's care? [y/N]"), REQUIRES_ANSWER))
        {
          team* Team = game::GetTeam(REBEL_TEAM);
          CrownPrince->ChangeTeam(Team);

          ADD_MESSAGE("\"Hi, uncle Harvan! Where are we? When are we gonna go home?\"");
          ADD_MESSAGE("\"Your Highness, I'm so very glad to see you. Don't worry, I will take you home soon.\"");
          game::SetRebelStoryState(game::GetRebelStoryState() + 1);
          return;
        }
      }
    }
  }

  humanoid::BeTalkedTo();
}

truth harvan::SpecialEnemySightedReaction(character* Char)
{
  if(!Char->IsPlayer() || GetPos().IsAdjacent(Char->GetPos()))
    return false;

  if(GetHP() > (GetMaxHP() / 2) && !RAND_N(10))
  {
    if(CanBeSeenByPlayer())
    {
      ADD_MESSAGE("%s screams at you: \"Get over here!\"", CHAR_NAME(DEFINITE));
    }
    else
      ADD_MESSAGE("\"Get over here!\"");

    Char->TeleportNear(this);
  }

  return false;
}

void lordregent::BeTalkedTo()
{
  if(GetPos().IsAdjacent(PLAYER->GetPos()) && !(GetRelation(PLAYER) == HOSTILE))
  {
    if(game::GetAslonaStoryState() == 1)
    {
      game::TextScreen(CONST_S("\"Sir Lancelyn sent you? He thought you can be of any use to me? I doubt that,\n"
                               "but then again maybe there is something in you, we shall see.\"\n\n"
                               "\"Several of my advisors have been complaining lately that we don't have enough\n"
                               "expendable personnel to send on special missions. Go see Lord Mittrars,\n"
                               "Myrddin the wizard and Senex of Seges, and report back once they are happy.\"\n\n"
                               "\"Now away with you, I have other things to worry about.\""));

      GetArea()->SendNewDrawRequest();
      ADD_MESSAGE("%s sighs: \"I don't have time for this.\"", CHAR_NAME(DEFINITE));
      game::SetAslonaStoryState(2);
      return;
    }
    else if(PLAYER->HasMuramasa())
    {
      if(game::TruthQuestion(CONST_S("Turn in the wicked katana named Asa'marum? [y/N]"), REQUIRES_ANSWER))
      {
        game::PlayVictoryMusic();
        game::TextScreen(CONST_S("You hand over the ancient katana and thus the regalia necessary to crown a new\n"
                                 "king of Aslona are together again.\n\n"
                                 "\"I am in your debt,\" Lord Peredivall says and then turns to his army. \"Citizens,\n"
                                 "this is our hour of victory. You have fought well. You are heroes, all of you.\n"
                                 "Our country and the very soul of our nation was saved only thanks to you, thanks\n"
                                 "to your bravery, honor, devotion and determination. But now, the war is over.\n"
                                 "The traitors shall face justice and peace will return to our homes. Gods bless Aslona!\"\n\n"
                                 "A deafening cheer echoes his words.\n\nYou are victorious!"));

        game::GetCurrentArea()->SendNewDrawRequest();
        game::DrawEverything();
        PLAYER->ShowAdventureInfo();

        // Did the player do all quests just for the royalists?
        festring Msg;
        if(game::GetAslonaStoryState() == 5)
        {
          Msg = CONST_S("helped the royalists to an overwhelming victory");
          AddScoreEntry(Msg, 4, false);
        }
        else
        {
          Msg = CONST_S("helped the royalists to an uneasy victory");
          AddScoreEntry(Msg, 2, false);
        }

        game::End(Msg);
        return;
      }
    }
    else if(game::GetAslonaStoryState() == 5 && game::GetStoryState() < 3)
    {
      game::TextScreen(CONST_S("\"Ah, you are back? I guess I owe you an apology. I expected you to try and leverage\n"
                               "some money from us, and then scram. But you really showed what you are made of!\n"
                               "I hope you could lend your services to Aslona one last time.\"\n\n"
                               "\"This civil war needs to end right now, before our kingdom tears itself apart, but\n"
                               "I don't like the options I see. The scouts of Lord Mittrars finally discovered\n"
                               "the location of Harvan's headquarters, but attacking them head-on in a difficult\n"
                               "terrain would be foolish and costly. Myrddin informed me he has a magical item\n"
                               "that could destroy the rebels once and for all, but I'm hesitant to condemn\n"
                               "every single one of those misguided souls to death.\""));

      game::TextScreen(CONST_S("\"Yet there is a third option. It is my sincere belief that many of Harvan's troops\n"
                               "would change sides if prince Artorius took the throne of his father as the rightful\n"
                               "king of Aslona. But for His Highness to be crowned the king without any doubts or\n"
                               "dispute, we need the regalia of Aslona, two ancient katanas of immaculate craft,\n"
                               "Asamarum and E-numa sa-am.\"\n\n"
                               "\"E-numa sa-am I already have, but Harvan absconded with Asamarum when he fled after\n"
                               "the old king's death. And where a direct assault would be hard-pressed for victory,\n"
                               "I think you as an outlander could slip through the sentries of the rebels' camp\n"
                               "unaccosted and steal Asamarum from Harvan.\"\n\n"
                               "\"A stealth mission, if you will.\""));

      GetArea()->SendNewDrawRequest();
      PLAYER->GetTeam()->Hostility(game::GetTeam(REBEL_TEAM)); // So much for a stealth mission. ;)
      ADD_MESSAGE("%s smiles at you: \"Together, we'll bring Harvan to justice.\"", CHAR_NAME(DEFINITE));
      game::SetStoryState(3);
      return;
    }
    else
    {
      // Does the player have prince Artorius in his team?
      character* CrownPrince = 0;
      for(character* p : game::GetTeam(PLAYER_TEAM)->GetMember())
        if(p->IsEnabled() && !p->IsPlayer() && p->IsKing())
          CrownPrince = p;

      if(CrownPrince)
      {
        ADD_MESSAGE("%s bows his head slightly. \"Your Highness, it is excellent to see you safe and sound. Please, hurry to tell Lord Mittrars about your return. He was truly sick with worry.\"", CHAR_NAME(DEFINITE));
        return;
      }
      else if(PLAYER->HasNuke() || PLAYER->HasWeepObsidian())
      {
        ADD_MESSAGE("\"Ah, seems like you were not idle. I'm sure my advisors will be thrilled.\"");
        return;
      }
    }
  }

  humanoid::BeTalkedTo();
}

struct distancepair
{
  distancepair(long Distance, character* Char) : Distance(Distance), Char(Char) { }
  bool operator<(const distancepair& D) const { return Distance > D.Distance; }
  long Distance;
  character* Char;
};

void lordregent::SpecialBodyPartSeverReaction()
{
  if(HasHead())
  {
    if(CanBeSeenByPlayer())
    {
      ADD_MESSAGE("%s prays to Seges. You feel the sudden presence of enemies.", CHAR_NAME(DEFINITE));
    }

    // Summons allies and then teleports away.
    std::vector<distancepair> ToSort;
    v2 Pos = GetPos();

    for(character* p : GetTeam()->GetMember())
      if(p->IsEnabled() && p != this)
        ToSort.push_back(distancepair((Pos - p->GetPos()).GetLengthSquare(), p));

    if(ToSort.size() > 5)
      std::sort(ToSort.begin(), ToSort.end());

    for(uint c = 0; c < 5 && c < ToSort.size(); ++c)
      ToSort[c].Char->TeleportNear(this);

    /* Teleport away, but rather than doing it the simple way, we're going to use
     * a teleport beam to also remove any items on the square. In effect, if the
     * player cuts off Lord Efra's sword arm, he will teleport and so will
     * Masamune, rather than for it to stay lying on the ground.
     */
    beamdata Beam
      (
        this,
        CONST_S("killed by the fickle favor of Seges"),
        YOURSELF,
        0
      );
    GetLSquareUnder()->Teleport(Beam);
    //TeleportRandomly(true);
  }
}

void child::BeTalkedTo()
{
  if(GetConfig() == KING &&
     GetRelation(PLAYER) != HOSTILE &&
     GetTeam() != PLAYER->GetTeam() &&
     GetDungeon()->GetIndex() == GOBLIN_FORT &&
     GetLevel()->GetIndex() == KING_LEVEL &&
     GetPos().IsAdjacent(PLAYER->GetPos())
   ) // Prince Artorius will follow you back to Aslona.
  {
    ADD_MESSAGE("%s looks at you with hope. \"I want to go home. Will you take me home, %s?\"",
                CHAR_DESCRIPTION(DEFINITE), PLAYER->GetAssignedName().CStr());

    ChangeTeam(PLAYER->GetTeam());
    return;
  }

  character::BeTalkedTo();
}

truth child::MoveRandomly()
{
  return GetConfig() == KING ? MoveRandomlyInRoom() : humanoid::MoveRandomly();
}
