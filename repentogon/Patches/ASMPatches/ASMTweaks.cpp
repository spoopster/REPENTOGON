#include "ASMPatcher.hpp"
#include "ASMTweaks.h"
#include "Log.h"
#include "IsaacRepentance.h"
#include "SigScan.h"
#include "../../REPENTOGONOptions.h"

namespace ASMPatches {
	static void __stdcall __TearDetonatorPatch(EntityList_EL*);

	bool FixGodheadEntityPartition() {
		SigScan signature("6aff56f3??????????????50f3??????????????e8????????8b");
		if (!signature.Scan()) {
			return false;
		}

		void* addr = signature.GetAddress();
		ASMPatch patch;
		patch.AddBytes(ByteBuffer().AddString("\x6a\x08"));
		sASMPatcher.FlatPatch(addr, &patch);

		return true;
	}

	bool FixTearDetonatorEntityList() {
		using Reg = ASMPatch::SavedRegisters;

		/* There is no decent place to patch. Ideally, checking if the list is empty
		 * before performing the patch should be done, but the game begins the loop
		 * immediately after performing such a check.
		 * 
		 * Patch immediately after acquiring the list instead. Tear Detonator should
		 * not be fired too many times either way.
		 */
		SigScan signature("c745fc1a0000008d8d????????33??89bd");
		if (!signature.Scan()) {
			return false;
		}

		void* addr = signature.GetAddress();
		ASMPatch patch;
		Reg registers(Reg::XMM_REGISTERS | Reg::GP_REGISTERS_STACKLESS, true);
		patch.PreserveRegisters(registers);
		patch.Push(ASMPatch::Registers::EAX); // eax contains the address of the EntityList to modify
		patch.AddInternalCall(__TearDetonatorPatch);
		patch.RestoreRegisters(registers);
		patch.AddRelativeJump((char*)addr + 0x7);

		sASMPatcher.PatchAt(addr, &patch);

		return true;
	}

	/* Change the content of the entity list so that it is no longer shared with other
	 * lists.
	 */
	void __stdcall __TearDetonatorPatch(EntityList_EL* list) {
		list->Untie();
	}

	/* Planetarium chance was implemented, but disabled! Oops!
	*/
	bool PatchStatHudPlanetariumChanceIcon() {
		SigScan signature("83fe080f84????????83fe090f84????????83fe06");
		if (!signature.Scan()) {
			return false;
		}
		
		SigScan jmpSig("4683fe0a0f82????????33d2");
		if (!jmpSig.Scan()) {
			return false;
		}

		void* addr = signature.GetAddress();
		void* jmpAddr = jmpSig.GetAddress();
		void* statHUDPlanetariumAddr = &repentogonOptions.statHUDPlanetarium;

		printf("[REPENTOGON] Patching planetarium chance icon visibility at %p", addr);
		ASMPatch patch;

		ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::Registers::EAX, true);
		patch.PreserveRegisters(savedRegisters)
			.AddBytes("\xA0").AddBytes(ByteBuffer().AddAny((char*)&statHUDPlanetariumAddr, 4)) // mov al, byte ptr ds:[XXXXXXXX]
			.AddBytes("\x84\xC0") // test al, al
			.RestoreRegisters(savedRegisters)
			.AddConditionalRelativeJump(ASMPatcher::CondJumps::JNE, jmpAddr)
			.AddRelativeJump((char*)addr + 0x8);
		sASMPatcher.FlatPatch(addr, &patch);

		return true;
	}
}