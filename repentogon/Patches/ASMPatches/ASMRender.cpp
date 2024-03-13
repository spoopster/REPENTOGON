#include "ASMPatcher.hpp"
#include "../ASMPatches.h"

// Planetarium chance was implemented, but disabled! Oops!
bool renderStatHUDPlanetarium = false;
void __stdcall SetStatHUDPlanetarium() {
	renderStatHUDPlanetarium = g_Manager->GetPersistentGameData()->Unlocked(406) && repentogonOptions.statHUDPlanetarium; //TODO enum
}

void PatchStatHudPlanetariumChanceIcon() {
	SigScan signature("83fe080f84????????83fe090f84????????83fe06");
	signature.Scan();

	SigScan jmpSig("4683fe0a0f82????????33d2");
	jmpSig.Scan(); 

	void* addr = signature.GetAddress();
	void* jmpAddr = jmpSig.GetAddress();
	void* statHUDPlanetariumAddr = &renderStatHUDPlanetarium;
	printf("[REPENTOGON] Patching planetarium chance icon visibility at %p\n", addr);
	ASMPatch patch;

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.AddBytes("\x83\xFE\x08") // cmp index, 0x8
		.AddBytes("\x75\x1E") // jne (current addr + 0x1E)
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(SetStatHUDPlanetarium)
		.AddBytes("\xA0").AddBytes(ByteBuffer().AddAny((char*)&statHUDPlanetariumAddr, 4)) // mov al, byte ptr ds:[XXXXXXXX]
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)jmpAddr)
		.AddRelativeJump((char*)addr + 0x9);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchStatHudPlanetariumChanceText() {
	SigScan signature("83fe080f84????????83fe090f84????????83fe07");
	signature.Scan();

	SigScan jmpSig("4683fe0a0f82????????8b5c24");
	jmpSig.Scan();

	void* addr = signature.GetAddress();
	void* jmpAddr = jmpSig.GetAddress();
	void* statHUDPlanetariumAddr = &renderStatHUDPlanetarium;

	printf("[REPENTOGON] Patching planetarium chance text visibility at %p\n", addr);
	ASMPatch patch;

	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.AddBytes("\x83\xFE\x08") // cmp index, 0x8
		.AddBytes("\x75\x19") // jne (current addr + 0x19)
		.PreserveRegisters(savedRegisters)
		.AddBytes("\xA0").AddBytes(ByteBuffer().AddAny((char*)&statHUDPlanetariumAddr, 4)) // mov al, byte ptr ds:[XXXXXXXX]
		.AddBytes("\x84\xC0") // test al, al
		.RestoreRegisters(savedRegisters)
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)jmpAddr)
		.AddRelativeJump((char*)addr + 0x9);
	sASMPatcher.PatchAt(addr, &patch);
}

void PatchStatHudPlanetariumChanceStr() {
	SigScan signature("83fe0774??f30f1044");
	signature.Scan();

	SigScan jmpSig("f30f1044????8d4424??0f5ac083ec08f20f11042468????????6a0c50e8????????b8????????b9????????f30f1044");
	jmpSig.Scan();

	void* addr = signature.GetAddress();
	void* jmpAddr = jmpSig.GetAddress();

	printf("[REPENTOGON] Patching planetarium chance string at %p\n", addr);
	ASMPatch patch;

	patch.AddBytes("\x83\xFE\x07") // cmp index, 0x7
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)jmpAddr)
		.AddBytes("\x83\xFE\x08") // cmp index, 0x8
		.AddConditionalRelativeJump(ASMPatcher::CondJumps::JE, (char*)jmpAddr)
		.AddRelativeJump((char*)addr + 0x5);

	sASMPatcher.PatchAt(addr, &patch);
}

float statHudPlanetariumChance = 0.0f;
void __stdcall SetStatHUDPlanetariumChance() {
	statHudPlanetariumChance = g_Game->GetPlanetariumChance() * 100;
}

void PatchStatHudPlanetariumChanceCalc() {
	SigScan signature("8d8e????????0f57c953");
	signature.Scan();

	SigScan flagSig("a90001000074??8d8e");
	flagSig.Scan();

	void* addr = signature.GetAddress();
	void* flagAddr = flagSig.GetAddress();
	void* planetariumStat = &statHudPlanetariumChance;

	printf("[REPENTOGON] Patching planetarium chance calculation at %p, %p\n", addr, flagAddr);
	ASMPatch patch;
	ASMPatch::SavedRegisters savedRegisters(ASMPatch::SavedRegisters::GP_REGISTERS_STACKLESS, true);
	patch.AddBytes("\x8D\x8E\xB8\x01").AddZeroes(2) // lea this, [esi + 0x1b8]
		.PreserveRegisters(savedRegisters)
		.AddInternalCall(SetStatHUDPlanetariumChance)
		.RestoreRegisters(savedRegisters)
		.AddBytes("\xF3\x0F\x10\x0D").AddBytes(ByteBuffer().AddAny((char*)&planetariumStat, 4)) // movss xmm1, dword ptr ds:[0xXXXXXXXX])
		.AddRelativeJump((char*)addr + 0x9);

	sASMPatcher.PatchAt(addr, &patch);

	ASMPatch flagPatch;
	flagPatch.AddBytes("\x90\x90\x90\x90\x90\x90\x90");

	sASMPatcher.FlatPatch(flagAddr, &flagPatch);

}

void PatchStatHudPlanetariumChance() {
	PatchStatHudPlanetariumChanceIcon();
	PatchStatHudPlanetariumChanceText();
	PatchStatHudPlanetariumChanceStr();
	PatchStatHudPlanetariumChanceCalc();
}