/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ScriptData
SDName: Instance_Karazhan
SD%Complete: 70
SDComment: Instance Script for Karazhan to help in various encounters. TODO: GameObject visibility for Opera event.
SDCategory: Karazhan
EndScriptData */

#include "ScriptPCH.h"
#include "karazhan.h"

#define ENCOUNTERS      12

/*
0  - Attumen + Midnight (optional)
1  - Moroes
2  - Maiden of Virtue (optional)
3  - Hyakiss the Lurker /  Rokad the Ravager  / Shadikith the Glider
4  - Opera Event
5  - Curator
6  - Shade of Aran (optional)
7  - Terestian Illhoof (optional)
8  - Netherspite (optional)
9  - Chess Event
10 - Prince Malchezzar
11 - Nightbane
*/

struct instance_karazhan : public ScriptedInstance
{
    instance_karazhan(Map* map) : ScriptedInstance(map) {Initialize();}

    uint32 Encounters[ENCOUNTERS];

    uint32 OperaEvent;
    uint32 OzDeathCount;

    uint64 CurtainGUID;
    uint64 StageDoorLeftGUID;
    uint64 StageDoorRightGUID;
    uint64 KilrekGUID;
    uint64 TerestianGUID;
    uint64 MoroesGUID;
    uint64 NightBaneGUID;

    uint64 SideEntranceDoor;
    uint64 ServantsAccessDoor;                              // Door to Brocken Stair
    uint64 LibraryDoor;                                     // Door at Shade of Aran
    uint64 MassiveDoor;                                     // Door at Netherspite
    uint64 GamesmansDoor;                                   // Door before Chess
    uint64 GamesmansExitDoor;                               // Door after Chess
    uint64 NetherspaceDoor;                                 // Door at Malchezaar
    uint64 MastersTerraceDoor[2];
    uint64 ImageGUID;

    void Initialize()
    {
        for (uint8 i = 0; i < ENCOUNTERS; ++i)
            Encounters[i] = NOT_STARTED;

        OperaEvent          = urand(1,3);                   // 1 - OZ, 2 - HOOD, 3 - RAJ, this never gets altered.
        OzDeathCount    = 0;

        SideEntranceDoor    = 0;
        ServantsAccessDoor  = 0;

        CurtainGUID         = 0;
        StageDoorLeftGUID   = 0;
        StageDoorRightGUID  = 0;

        KilrekGUID          = 0;
        TerestianGUID       = 0;
        MoroesGUID          = 0;
        NightBaneGUID       = 0;

        LibraryDoor         = 0;
        MassiveDoor         = 0;
        GamesmansDoor       = 0;
        GamesmansExitDoor   = 0;
        NetherspaceDoor     = 0;
        MastersTerraceDoor[0]= 0;
        MastersTerraceDoor[1]= 0;
        ImageGUID = 0;
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < ENCOUNTERS; ++i)
            if (Encounters[i] == IN_PROGRESS)
                return true;

        return false;
    }

    uint32 GetData(uint32 identifier)
    {
        switch (identifier)
        {
            case DATA_ATTUMEN_EVENT:          return Encounters[0];
            case DATA_MOROES_EVENT:           return Encounters[1];
            case DATA_MAIDENOFVIRTUE_EVENT:   return Encounters[2];
            case DATA_OPTIONAL_BOSS_EVENT:    return Encounters[3];
            case DATA_OPERA_EVENT:            return Encounters[4];
            case DATA_CURATOR_EVENT:          return Encounters[5];
            case DATA_SHADEOFARAN_EVENT:      return Encounters[6];
            case DATA_TERESTIAN_EVENT:        return Encounters[7];
            case DATA_NETHERSPITE_EVENT:      return Encounters[8];
            case DATA_CHESS_EVENT:            return Encounters[9];
            case DATA_MALCHEZZAR_EVENT:       return Encounters[10];
            case DATA_NIGHTBANE_EVENT:        return Encounters[11];
            case DATA_OPERA_PERFORMANCE:      return OperaEvent;
            case DATA_OPERA_OZ_DEATHCOUNT:    return OzDeathCount;
            case DATA_IMAGE_OF_MEDIVH:        return ImageGUID;
        }

        return 0;
    }

    void OnCreatureCreate(Creature *creature, uint32 entry)
    {
        switch (creature->GetEntry())
        {
            case 17229:   KilrekGUID = creature->GetGUID();      break;
            case 15688:   TerestianGUID = creature->GetGUID();   break;
            case 15687:   MoroesGUID = creature->GetGUID();      break;
        }
    }

    uint64 GetData64(uint32 data)
    {
        switch (data)
        {
            case DATA_KILREK:                      return KilrekGUID;
            case DATA_TERESTIAN:                   return TerestianGUID;
            case DATA_MOROES:                      return MoroesGUID;
            case DATA_GAMEOBJECT_STAGEDOORLEFT:    return StageDoorLeftGUID;
            case DATA_GAMEOBJECT_STAGEDOORRIGHT:   return StageDoorRightGUID;
            case DATA_GAMEOBJECT_CURTAINS:         return CurtainGUID;
            case DATA_GAMEOBJECT_LIBRARY_DOOR:     return LibraryDoor;
            case DATA_GAMEOBJECT_MASSIVE_DOOR:     return MassiveDoor;
            case DATA_GAMEOBJECT_GAME_DOOR:        return GamesmansDoor;
            case DATA_GAMEOBJECT_GAME_EXIT_DOOR:   return GamesmansExitDoor;
            case DATA_GAMEOBJECT_NETHER_DOOR:      return NetherspaceDoor;
            case DATA_IMAGE_OF_MEDIVH:             return ImageGUID;
            case DATA_MASTERS_TERRACE_DOOR_1:      return MastersTerraceDoor[0];
            case DATA_MASTERS_TERRACE_DOOR_2:      return MastersTerraceDoor[1];
            case DATA_NIGHTBANE:                   return NightBaneGUID;
        }

        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch (type)
        {
            case DATA_ATTUMEN_EVENT:
                if (Encounters[0] != DONE)
                    Encounters[0]  = data;
                break;
            case DATA_MOROES_EVENT:
                if (Encounters[1] != DONE)
                    Encounters[1] = data;
                break;
            case DATA_MAIDENOFVIRTUE_EVENT:
                if (Encounters[2] != DONE)
                    Encounters[2]  = data;
                break;
            case DATA_OPTIONAL_BOSS_EVENT:
                if (Encounters[3] != DONE)
                    Encounters[3]  = data;
                break;
            case DATA_OPERA_EVENT:
                if (Encounters[4] != DONE)
                    Encounters[4]  = data;
                break;
            case DATA_CURATOR_EVENT:
                if (Encounters[5] != DONE)
                    Encounters[5]  = data;
                break;
            case DATA_SHADEOFARAN_EVENT:
                if (Encounters[6] != DONE)
                    Encounters[6]  = data;
                break;
            case DATA_TERESTIAN_EVENT:
                if (Encounters[7] != DONE)
                    Encounters[7]  = data;
                break;
            case DATA_NETHERSPITE_EVENT:
                if (Encounters[8] != DONE)
                    Encounters[8]  = data;
                break;
            case DATA_CHESS_EVENT:
                if (Encounters[9] != DONE)
                    Encounters[9]  = data;
                break;
            case DATA_MALCHEZZAR_EVENT:
                if (Encounters[10] != DONE)
                    Encounters[10] = data;
                break;
            case DATA_NIGHTBANE_EVENT:
                if (Encounters[11] != DONE)
                    Encounters[11] = data;
                break;
            case DATA_OPERA_OZ_DEATHCOUNT:
                ++OzDeathCount;
                break;
        }

        if (data == DONE)
            SaveToDB();
    }

    void SetData64(uint32 identifier, uint64 data)
    {
        switch(identifier)
        {
            case DATA_IMAGE_OF_MEDIVH: ImageGUID = data;
            case DATA_NIGHTBANE:       NightBaneGUID = data;
        }
    }

    void OnObjectCreate(GameObject* go)
    {
        switch(go->GetEntry())
        {
            case 184281:   ServantsAccessDoor   = go->GetGUID();         break;
            case 184275:   SideEntranceDoor     = go->GetGUID();         break;
            case 183932:   CurtainGUID          = go->GetGUID();         break;
            case 184278:   StageDoorLeftGUID    = go->GetGUID();         break;
            case 184279:   StageDoorRightGUID   = go->GetGUID();         break;
            case 184517:   LibraryDoor          = go->GetGUID();         break;
            case 185521:   MassiveDoor          = go->GetGUID();         break;
            case 184276:   GamesmansDoor        = go->GetGUID();         break;
            case 184277:   GamesmansExitDoor    = go->GetGUID();         break;
            case 185134:   NetherspaceDoor      = go->GetGUID();         break;
            case 184274:   MastersTerraceDoor[0] = go->GetGUID();        break;
            case 184280:   MastersTerraceDoor[1] = go->GetGUID();        break;
        }

        switch(OperaEvent)
        {
            //TODO: Set Object visibilities for Opera based on performance
            case EVENT_OZ:
                break;

            case EVENT_HOOD:
                break;

            case EVENT_RAJ:
                break;
        }
    }

    std::string GetSaveData()
    {
        OUT_SAVE_INST_DATA;
        std::ostringstream stream;
        stream << "K Z " << " "
            << Encounters[0] << " " << Encounters[1] << " "
            << Encounters[2] << " " << Encounters[3] << " "
            << Encounters[4] << " " << Encounters[5] << " "
            << Encounters[6] << " " << Encounters[7] << " "
            << Encounters[8] << " " << Encounters[9] << " "
            << Encounters[10] << " " << Encounters[11];
        char* out = new char[stream.str().length() + 1];
        strcpy(out, stream.str().c_str());
        if (out)
        {
            OUT_SAVE_INST_DATA_COMPLETE;
            return out;
        }

        return NULL;
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);
        std::istringstream stream(in);
        char dataHead1, dataHead2;
        stream >> dataHead1 >> dataHead2 >>
            Encounters[0] >> Encounters[1] >>
            Encounters[2] >> Encounters[3] >>
            Encounters[4] >> Encounters[5] >>
            Encounters[6] >> Encounters[7] >>
            Encounters[8] >> Encounters[9] >>
            Encounters[10] >> Encounters[11];
        if (dataHead1 != 'K' || dataHead2 != 'Z')
        {
            error_log("SD2: Karazhan corrupted save data.");
            for (int i = 0; i < ENCOUNTERS; i++)
                Encounters[i] = 0;
        } else OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_karazhan(Map* map)
{
    return new instance_karazhan(map);
}

void AddSC_instance_karazhan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_karazhan";
    newscript->GetInstanceData = &GetInstanceData_instance_karazhan;
    newscript->RegisterSelf();
}

