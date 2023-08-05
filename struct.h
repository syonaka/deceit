#pragma once
#include "include.h"

typedef __int64 EntityId;

namespace str
{
	class IConsole
	{
	public:
		void ExecuteString(const char* command, const bool bSilentMode = false, const bool bDeferExecution = false)
		{
			return CallVFunction<void(__fastcall*)(PVOID, const char*, const bool, const bool)>(this, 25)(this, command, bSilentMode, bDeferExecution);
		}
	};

	class IRender
	{
	public:
		bool ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz)
		{
			typedef bool(__thiscall* oFunc)(void*, float, float, float, float*, float*, float*);
			bool ret = CallVFunction< oFunc >(this, 71)(this, ptx, pty, ptz, sx, sy, sz);
			*sx *= (trash::RenderWidth / 100.0f);
			*sy *= (trash::RenderHeight / 100.0f);
			*sz *= 1.0f;

			return (*sz < 1.0f);
		}

		bool UNProjectFromScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz)
		{
			typedef bool(__thiscall* oFunc)(void*, float, float, float, float*, float*, float*);
			bool ret = CallVFunction< oFunc >(this, 73)(this, ptx, pty, ptz, sx, sy, sz);

			return ret;
		}

		Vector3 ProjectToScreen(Vector3 in) {
			Vector3 out;
			this->ProjectToScreen(in, &out);
			return out;
		}

		bool ProjectToScreen(Vector3 in, Vector3* out) {
			float x, y, z;
			if (!ProjectToScreen(in.x, in.y, in.z, &x, &y, &z))
				return false;
			out->x = x;
			out->y = y;
			out->z = z;
			return true;
		}

		void ShutDownFast()
		{
			CallVFunction<void(__thiscall*)(void*)>(this, 38)(this);
		}
	};

	enum EEvent {
		TransformChanged, TransformChangeDone, TimerExpired, Initialize,
		Remove, Reset, ChildAttached, AttachedToParent,
		ChildDetached, DetachedFromParent, LinkAdded, LinkRemoved,
		Hidden, Unhidden, LayerHidden, LayerUnhidden,
		PhysicsToggled, PhysicsStateChanged, LuaScriptEvent, OtherEntityEnteredArea,
		OtherEntityLeftArea, OtherEntityEnteredNearArea, OtherEntityLeftNearArea, OtherEntityMoveInsideArea,
		OtherEntityMoveNearArea, PhysicsThreadPostStep, PhysicalObjectBroken, PhysicsCollision,
		PhysicsThreadCollision, RenderVisibilityChanged, LevelLoaded, LevelStarted,
		GameplayStarted, EnterLuaScriptState, LeaveLuaScriptState, DeserializeSaveGameStart,
		DeserializeSaveGameDone, BecomeInvisible, BecomeVisible, OverrideMaterialChanged,
		MaterialLayerChanged, AnimationEvent, LuaScriptSetColliderMode, ActivateFlowNodeOutput,
		EditorPropertyChanged, ReloadLuaScript, SubscribedForUpdates, UnsubscribedFromUpdates,
		NetworkAuthorityChanged, BecomeLocalPlayer, AddToRadar, RemoveFromRadar,
		NameChanged, AudioTriggerStarted, AudioTriggerEnded, SlotChanged,
		PhysicalTypeChanged, NetworkReplicatedFromServer, PrePhysicsUpdate, Update,
		Last
	};

	struct SEntityEvent
	{
		SEntityEvent(
			const int n0,
			const int n1,
			const int n2,
			const int n3,
			const float f0,
			const float f1,
			const float f2,
			Vector3 const& _vec)
		{
			nParam[0] = n0;
			nParam[1] = n1;
			nParam[2] = n2;
			nParam[3] = n3;
			fParam[0] = f0;
			fParam[1] = f1;
			fParam[2] = f2;
		}

		SEntityEvent()
		{
			nParam[0] = nParam[1] = nParam[2] = nParam[3] = 0;
			fParam[0] = fParam[1] = fParam[2] = 0.0f;
		}

		EEvent eventQ; //!< Any event from EEntityEvent enum.
		intptr_t nParam[4]; //!< Event parameters.
		float fParam[3];
		Vector3 vec;
	};

	struct SEntitySpawnParams
	{

	};

	struct IEntityClass 
	{
		enum EventValueType 
		{
			EVT_INT,
			EVT_FLOAT,
			EVT_BOOL,
			EVT_VECTOR,
			EVT_ENTITY,
			EVT_STRING
		};
		struct SEventInfo 
		{
			const char* name;
			EventValueType type;
			bool bOutput;
		};
	};

	class IEntityLayer
	{
	public:
		IEntityLayer() {}
		virtual ~IEntityLayer() {}

		virtual void          SetParentName(const char* szParent) = 0;
		virtual void          AddChild(IEntityLayer* pLayer) = 0;
		virtual int           GetNumChildren() const = 0;
		virtual IEntityLayer* GetChild(int idx) const = 0;
		virtual void          AddObject(EntityId id) = 0;
		virtual void          RemoveObject(EntityId id) = 0;
		virtual void          Enable(bool bEnable, bool bSerialize = true, bool bAllowRecursive = true) = 0;
		virtual bool          IsEnabled() const = 0;
		virtual bool          IsEnabledBrush() const = 0;
		virtual bool          IsSerialized() const = 0;
		virtual bool          IsDefaultLoaded() const = 0;
		virtual bool          IncludesEntity(EntityId id) const = 0;
		virtual const char*   GetName() const = 0;
		virtual const char*   GetParentName() const = 0;
		virtual uint16_t	  GetId() const = 0;
		virtual bool          IsSkippedBySpec() const = 0;
	};

	class IEntity
	{
	public:
		EntityId GetId() 
		{
			auto func = CallVFunction<EntityId(__thiscall*)(void*)>(this, 1);
			return func(this);
		}

		std::string GetClass() 
		{
			uintptr_t entityInfo = *(uintptr_t*)((uintptr_t)this + 0x18);
			auto entityPtr = *(char**)(entityInfo + 0x10);
			return std::string(entityPtr);
		}

		void SetScale(Vector3 vScl) 
		{
			int check{ 0 };
			CallVFunction<IEntityClass* (__thiscall*)(void*, Vector3, int* unkwn)>(this, 41)(this, vScl, &check);
		}

		void SetRotation(Quat vScl) 
		{
			int check{ 0 };
			CallVFunction<IEntityClass* (__thiscall*)(void*, Quat, int* unkwn)>(this, 39)(this, vScl, &check);
		}

		Quat* GetRotation()
		{
			return CallVFunction<Quat* (__thiscall*)(void*)>(this, 40)(this);
		}

		Vector3* GetScale()
		{
			return CallVFunction<Vector3 * (__thiscall*)(void*)>(this, 42)(this);
		}

		union 
		{
			struct 
			{
				BYTE pad[(DWORD)0x10];
				char* Name;
			};

			struct 
			{
				BYTE pad1[(DWORD)0xDC];
				Vector3 Position;
			};
		};
	};

	class IEntityIt
	{
	public:
		virtual ~IEntityIt() {}
		virtual void AddRef() = 0;
		virtual void Release() = 0;
		virtual bool IsEnd() = 0;
		virtual IEntity* Next() = 0;
		virtual IEntity* This() = 0;
		virtual void MoveFirst() = 0;
	};

	class IMeleeParam 
	{
	public:
		union {
			struct
			{
				BYTE pad2[(DWORD)0x08];
				float Range;
			};
			struct
			{
				BYTE pad22[(DWORD)0x28];
				float Delay;
			};
			struct
			{
				BYTE pad25[(DWORD)0x30];
				float Duration;
			};
		};
	};

	class IMelee 
	{
	public:
		union {
			struct
			{
				BYTE pad2[(DWORD)0x18];
				IMeleeParam* MeleeParam;
			};
		};
	};

	class IFireMode
	{
	public:

	};
	
	class IZoomMode
	{
	public:

	};

	class IWeapon
	{
	public:
		union
		{
			struct
			{
				BYTE pad2[(DWORD)0x40];
				IMelee* Melee;
			};
		};

		void StartFire()
		{
			CallVFunction<void(__thiscall*)(void*)>(this, 12)(this);
		}

		void StopFire()
		{
			CallVFunction<void(__thiscall*)(void*)>(this, 13)(this);
		}

		void StartZoom(EntityId shooterId, int zoomed = 1)
		{
			CallVFunction<void(__thiscall*)(void*, EntityId, int)>(this, 16)(this, shooterId, zoomed);
		}

		void StopZoom(EntityId shooterId)
		{
			CallVFunction<void(__thiscall*)(void*, EntityId)>(this, 17)(this, shooterId);
		}

		bool CanZoom()
		{
			return CallVFunction<bool(__thiscall*)(void*)>(this, 18)(this);
		}

		void ExitZoom(bool force = false)
		{
			CallVFunction<void(__thiscall*)(void*, bool)>(this, 19)(this, force);
		}

		void Reload(bool force = false)
		{
			CallVFunction<void(__thiscall*)(void*, bool)>(this, 20)(this, force);
		}

		bool CanReload()
		{
			return CallVFunction<bool(__thiscall*)(void*)>(this, 21)(this);
		}

		int GetAmmoCount(IEntityClass* pAmmoType)
		{
			return CallVFunction<int(__thiscall*)(void*, IEntityClass*)>(this, 25)(this, pAmmoType);
		}

		void SetAmmoCount(IEntityClass* pAmmoType, int count)
		{
			CallVFunction<void(__thiscall*)(void*, IEntityClass*, int)>(this, 26)(this, pAmmoType, count);
		}
	};

	class IItem
	{
	public:
		IWeapon* GetWeapon() 
		{
			return CallVFunction<IWeapon * (__fastcall*)(void*)>(this, 90)(this);
		}

		std::string GetName() 
		{
			char* nameP = (char*)(*(uintptr_t*)(*(uintptr_t*)(this + 16) + 16));
			return std::string(nameP);
		}
	};

	class IActor
	{
	public:

		IItem* GetCurrentItem(bool includeVehicle = false) 
		{
			if (!this)
				return nullptr;
			return CallVFunction <IItem * (__thiscall*)(void*, bool)>(this, 71)(this, includeVehicle);
		}

		union
		{
			struct
			{
				BYTE pad23[(DWORD)0X10];
				IEntity* entity;
			};

			struct
			{
				BYTE pad2[(DWORD)0x88];
				float Health;
			};
		};
	};

	class IActorIterator
	{
	public:
		virtual ~IActorIterator() {}
		virtual size_t  Count() = 0;
		virtual IActor* Next() = 0;
		virtual void    AddRef() = 0;
		virtual void    Release() = 0;
	};

	class ISystem
	{
	public:
		void DisplayErrorMessage(const char* acMessage, float fTime, const float* pfColor = 0, bool bHardError = true)
		{
			CallVFunction<void(__thiscall*)(void*, const char*, float, const float*, bool)>(this, 27)(this, acMessage, fTime, pfColor, bHardError);
		}
	};

	class IEntitySystem
	{
	public:
		ISystem* GetSystem()
		{
			return CallVFunction<ISystem * (__thiscall*)(void*)>(this, 31)(this);
		}

		IEntityIt* GetEntityIterator()
		{
			int check{ 0 };
			return CallVFunction<IEntityIt * (__thiscall*)(void*, void*)>(this, 17)(this, &check);
		}

		void RemoveEntity(EntityId entity, bool bForceRemoveNow = false)
		{
			CallVFunction<void(__thiscall*)(void*, EntityId, bool)>(this, 15)(this, entity, bForceRemoveNow);
		}

		IEntity* SpawnEntity(SEntitySpawnParams& params, bool bAutoInit = true)
		{
			return CallVFunction<IEntity * (__thiscall*)(void*, SEntitySpawnParams&, bool)>(this, 9)(this, params, bAutoInit);
		}

		/* Reload IEntitySystem */
		void Reload()
		{
			CallVFunction<void(__thiscall*)(void*)>(this, 20)(this);
		}

		void SendEventToAll(SEntityEvent& e)
		{
			CallVFunction<void(__thiscall*)(void*, SEntityEvent&)>(this, 18)(this, e);
		}

		/* stop timer for each entity */
		void PauseTimers(bool bPause, bool bResume = false)
		{
			CallVFunction<void(__thiscall*)(void*, bool, bool)>(this, 28)(this, bPause, bResume);
		}

		IEntityLayer* AddLayer(const char* szName, const char* szParent, uint16_t id, bool bHasPhysics, int specs, bool bDefaultLoaded)
		{
			return CallVFunction<IEntityLayer * (__thiscall*)(void*, const char*, const char*, uint16_t, bool, int, bool)>(this, 58)(\
				this, szName, szParent, id, bHasPhysics, specs, bDefaultLoaded);
		}

		void RemoveEntityFromLayers(EntityId id)
		{
			CallVFunction<void(__thiscall*)(void*, EntityId)>(this, 62)(this, id);
		}

		void ClearLayers()
		{
			CallVFunction<void(__thiscall*)(void*)>(this, 63)(this);
		}

		void EnableDefaultLayers(bool isSerialized = true)
		{
			CallVFunction<void(__thiscall*)(void*, bool)>(this, 64)(this, isSerialized);
		}
	};

	class IActorSystem
	{
	public:
		void Reset() 
		{
			CallVFunction<IEntityClass* (__thiscall*)(void*)>(this, 1)(this);
		}

		void Reload() 
		{
			CallVFunction<IEntityClass* (__thiscall*)(void*)>(this, 2)(this);
		}

		IActor* GetActor(EntityId id) 
		{
			typedef IActor* (__thiscall* OriginalFunc)(void* self, EntityId id);
			return CallVFunction<OriginalFunc>(this, 3)(this, id);
		}

		IActorIterator* CreateActorIterator()
		{
			return CallVFunction<IActorIterator * (__thiscall*)(void*)>(this, 7)(this);
		}

		void AddActor(EntityId entityId, IActor* pActor)
		{
			CallVFunction<IEntityClass* (__thiscall*)(void*, EntityId, IActor*)>(this, 13)(this, entityId, pActor);
		}

		void RemoveActor(EntityId id) 
		{
			CallVFunction<IEntityClass* (__thiscall*)(void*, EntityId)>(this, 14)(this, id);
		}

		IActor* CreateActor(uint16_t channelId, const char* name, const char* actorClass, const Vector3& pos, const Quat& rot, const Vector3& scale, EntityId id)
		{
			return CallVFunction<IActor * (__thiscall*)(void*, uint16_t, const char*, const char*, const Vector3, Quat, Vector3, EntityId)>(this, 5)(\
				this, channelId, name, actorClass, pos, rot, scale, id);
		}


	};

	class IGameFramework
	{
	public:
		IEntity* GetClientEntity() 
		{
			return CallVFunction<IEntity * (__thiscall*)(void*)>(this, 69)(this);
		}

		IActorSystem* GetActorSystem() 
		{
			return CallVFunction<IActorSystem * (__thiscall*)(void*)>(this, 26)(this);
		}
	};
}