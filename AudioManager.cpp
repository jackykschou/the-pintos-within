#include "AudioManager.h"
#include "GameState.h"
#include "PlayerCharacter.h"

void musicDone();

void AudioManager::initialize() 
{
   in_pinto = false;

	SDL_Init(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS)) {
	    throw("Unable to open audio channel.");
	}

	current_music = NULL;
	pinto_music = Mix_LoadMUS("media/sounds/Pinto.wav");
	normal_music[0] = Mix_LoadMUS("media/sounds/Music1.wav");
	normal_music[1] = Mix_LoadMUS("media/sounds/Music2.wav");
	normal_music[2] = Mix_LoadMUS("media/sounds/Music3.wav");
	normal_music[3] = Mix_LoadMUS("media/sounds/Music4.wav");
	normal_music[4] = Mix_LoadMUS("media/sounds/Music5.wav");
	normal_music[5] = Mix_LoadMUS("media/sounds/Music6.wav");

	current_fire = 0;
	current_dust = 0;
	current_blood = 0;

	Mix_HookMusicFinished(musicDone);
	Mix_VolumeMusic(50);

	_donks[0] = AudioManager::instance()->loadAudioFile("media/sounds/Donk1.wav");
	_donks[1] = AudioManager::instance()->loadAudioFile("media/sounds/Donk2.wav");
	_donks[2] = AudioManager::instance()->loadAudioFile("media/sounds/Donk3.wav");
	_donks[3] = AudioManager::instance()->loadAudioFile("media/sounds/Donk4.wav");
	_startSound = AudioManager::instance()->loadAudioFile("media/sounds/Balls.wav");
	_ballzSound = AudioManager::instance()->loadAudioFile("media/sounds/BallBlasterIntro.wav");

	_rifleFire[0][0] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire1.wav");
	_rifleFire[0][1] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire2.wav");
	_rifleFire[0][2] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire3.wav");
	_rifleFire[0][3] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire4.wav");
	_rifleFire[0][4] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire5.wav");
	_rifleFire[0][5] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire6.wav");
	_rifleFire[0][6] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire7.wav");
	_rifleFire[0][7] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire8.wav");
	_rifleFire[0][8] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire9.wav");
	_rifleFire[0][9] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire10.wav");
	_rifleFire[0][10] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire11.wav");
	_rifleFire[0][11] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire12.wav");
	_rifleFire[0][12] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire13.wav");
	_rifleFire[0][13] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire14.wav");
	_rifleFire[0][14] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire15.wav");
	_rifleFire[0][15] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire1.wav");
	_rifleFire[0][16] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire2.wav");
	_rifleFire[0][17] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire3.wav");
	_rifleFire[0][18] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire4.wav");
	_rifleFire[0][19] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire5.wav");
	_rifleFire[0][20] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire6.wav");
	_rifleFire[0][21] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire7.wav");
	_rifleFire[0][22] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire8.wav");
	_rifleFire[0][23] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire9.wav");
	_rifleFire[0][24] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire10.wav");
	_rifleFire[0][25] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire11.wav");
	_rifleFire[0][26] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire12.wav");
	_rifleFire[0][27] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire13.wav");
	_rifleFire[0][28] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire14.wav");
	_rifleFire[0][29] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire15.wav");

	_rifleFire[1][0] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire1.wav");
	_rifleFire[1][1] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire2.wav");
	_rifleFire[1][2] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire3.wav");
	_rifleFire[1][3] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire4.wav");
	_rifleFire[1][4] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire5.wav");
	_rifleFire[1][5] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire6.wav");
	_rifleFire[1][6] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire7.wav");
	_rifleFire[1][7] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire8.wav");
	_rifleFire[1][8] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire9.wav");
	_rifleFire[1][9] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire10.wav");
	_rifleFire[1][10] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire11.wav");
	_rifleFire[1][11] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire12.wav");
	_rifleFire[1][12] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire13.wav");
	_rifleFire[1][13] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire14.wav");
	_rifleFire[1][14] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire15.wav");
	_rifleFire[1][15] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire1.wav");
	_rifleFire[1][16] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire2.wav");
	_rifleFire[1][17] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire3.wav");
	_rifleFire[1][18] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire4.wav");
	_rifleFire[1][19] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire5.wav");
	_rifleFire[1][20] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire6.wav");
	_rifleFire[1][21] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire7.wav");
	_rifleFire[1][22] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire8.wav");
	_rifleFire[1][23] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire9.wav");
	_rifleFire[1][24] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire10.wav");
	_rifleFire[1][25] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire11.wav");
	_rifleFire[1][26] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire12.wav");
	_rifleFire[1][27] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire13.wav");
	_rifleFire[1][28] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire14.wav");
	_rifleFire[1][29] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire15.wav");

	_rifleFire[2][0] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire1.wav");
	_rifleFire[2][1] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire2.wav");
	_rifleFire[2][2] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire3.wav");
	_rifleFire[2][3] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire4.wav");
	_rifleFire[2][4] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire5.wav");
	_rifleFire[2][5] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire6.wav");
	_rifleFire[2][6] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire7.wav");
	_rifleFire[2][7] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire8.wav");
	_rifleFire[2][8] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire9.wav");
	_rifleFire[2][9] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire10.wav");
	_rifleFire[2][10] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire11.wav");
	_rifleFire[2][11] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire12.wav");
	_rifleFire[2][12] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire13.wav");
	_rifleFire[2][13] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire14.wav");
	_rifleFire[2][14] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire15.wav");
	_rifleFire[2][15] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire16.wav");
	_rifleFire[2][16] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire17.wav");
	_rifleFire[2][17] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire18.wav");
	_rifleFire[2][18] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire19.wav");
	_rifleFire[2][19] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire20.wav");
	_rifleFire[2][20] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire21.wav");
	_rifleFire[2][21] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire22.wav");
	_rifleFire[2][22] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire23.wav");
	_rifleFire[2][23] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire24.wav");
	_rifleFire[2][24] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire25.wav");
	_rifleFire[2][25] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire26.wav");
	_rifleFire[2][26] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire27.wav");
	_rifleFire[2][27] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire28.wav");
	_rifleFire[2][28] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire29.wav");
	_rifleFire[2][29] = AudioManager::instance()->loadAudioFile("media/sounds/AssultRifleFire30.wav");

	_rifleFire[3][0] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][1] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][2] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][3] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][4] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][5] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][6] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][7] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][8] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][9] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][10] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][11] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][12] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][13] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][14] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][15] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][16] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][17] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][18] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][19] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][20] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][21] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][22] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][23] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][24] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][25] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][26] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][27] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][28] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][29] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");

	_rifleFire[4][0] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][1] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][2] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][3] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][4] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][5] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][6] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][7] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][8] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][9] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][10] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][11] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][12] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][13] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][14] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][15] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][16] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][17] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][18] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][19] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][20] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][21] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][22] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][23] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][24] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][25] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][26] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][27] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][28] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][29] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");

	_bulletDirtCollision[0] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision1.wav");
	_bulletDirtCollision[1] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision2.wav");
	_bulletDirtCollision[2] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision3.wav");
	_bulletDirtCollision[3] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision4.wav");
	_bulletDirtCollision[4] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision5.wav");
	_bulletDirtCollision[5] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision6.wav");
	_bulletDirtCollision[6] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision7.wav");
	_bulletDirtCollision[7] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision8.wav");
	_bulletDirtCollision[8] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision9.wav");
	_bulletDirtCollision[9] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision7.wav");
	_bulletDirtCollision[10] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision9.wav");
	_bulletDirtCollision[11] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision10.wav");
	_bulletDirtCollision[12] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision11.wav");
	_bulletDirtCollision[13] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision12.wav");
	_bulletDirtCollision[14] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision13.wav");
	_bulletDirtCollision[15] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision14.wav");
	_bulletDirtCollision[16] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision15.wav");
	_bulletDirtCollision[17] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision16.wav");
	_bulletDirtCollision[18] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision17.wav");
	_bulletDirtCollision[19] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision18.wav");
	_bulletDirtCollision[20] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision19.wav");
	_bulletDirtCollision[21] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision20.wav");
	_bulletDirtCollision[22] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision21.wav");
	_bulletDirtCollision[23] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision22.wav");
	_bulletDirtCollision[24] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision23.wav");
	_bulletDirtCollision[25] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision24.wav");

	_footstep[0] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep1.wav");
	_footstep[1] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep2.wav");
	_footstep[2] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep3.wav");
	_footstep[3] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep4.wav");
	_footstep[4] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep5.wav");
	_footstep[5] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep6.wav");
	_footstep[6] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep7.wav");

	_jetpack = AudioManager::instance()->loadAudioFile("media/sounds/Jetpack1.wav");

	_death = AudioManager::instance()->loadAudioFile("media/sounds/Death1.wav");

	_reloads[0] = AudioManager::instance()->loadAudioFile("media/sounds/PistolReload.wav");
	_reloads[1] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunReload.wav");
	_reloads[2] = AudioManager::instance()->loadAudioFile("media/sounds/AssaultReload.wav");
	_reloads[3] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterReload.wav");
	_reloads[4] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");

	_blood_splats[0] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat1.wav");
	_blood_splats[1] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat2.wav");
	_blood_splats[2] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat3.wav");
	_blood_splats[3] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat4.wav");
	_blood_splats[4] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat5.wav");
	_blood_splats[5] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat6.wav");
	_blood_splats[6] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat7.wav");
	_blood_splats[7] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat8.wav");
	_blood_splats[8] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat9.wav");
	_blood_splats[9] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat10.wav");
	_blood_splats[10] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat11.wav");
	_blood_splats[11] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat12.wav");
	_blood_splats[12] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat13.wav");
	_blood_splats[13] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat14.wav");
	_blood_splats[14] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat15.wav");
	_blood_splats[15] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat16.wav");
	_blood_splats[16] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat17.wav");
	_blood_splats[17] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat18.wav");
	_blood_splats[18] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat19.wav");
	_blood_splats[19] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat20.wav");
	_blood_splats[20] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat21.wav");
	_blood_splats[21] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat22.wav");
	_blood_splats[22] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat23.wav");
	_blood_splats[23] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat24.wav");

	_blaster_charge = AudioManager::instance()->loadAudioFile("media/sounds/BlasterCharge.wav");
	_out_of_ammo = AudioManager::instance()->loadAudioFile("media/sounds/OutOfAmmo.wav");
	_pick_weapon = AudioManager::instance()->loadAudioFile("media/sounds/PickWeapon.wav");

	_pinto_spawn = AudioManager::instance()->loadAudioFile("media/sounds/PintoSpawn.wav");
	_pinto_die = AudioManager::instance()->loadAudioFile("media/sounds/PintoDie.wav");
	_change_hair = AudioManager::instance()->loadAudioFile("media/sounds/ChangeHair.wav");
}

// returns an audioFileKey that references the loaded resource
AudioFile AudioManager::loadAudioFile(const char *file) {
	Mix_Chunk* c = Mix_LoadWAV(file);
	if (!c) {
		std::cout << "Failed to load file " << file << std::endl;
		printf("Error Mix_LoadWAV: %s\n", Mix_GetError());
		return -1;
	}
	_loadedFiles.push_back(c);
	return _loadedFiles.size() - 1;
}

// returns a channel integer that can be used to stop the sound
AudioChannel AudioManager::play2DSound(AudioFile file, int loops)
{
	return Mix_PlayChannel(-1, _loadedFiles[file], loops);
}

void AudioManager::play3DSound(AudioFile file, int loops, Ogre::Vector3 v, uint8_t sound_volume)
{
	if(GameState::instance()->player == NULL)
		return;
	
	Ogre::Vector3 curPos = Ogre::Vector3(GameState::instance()->player->tr->posX, GameState::instance()->player->tr->posY, GameState::instance()->player->tr->posZ);
	
	//max dist = 100m 
	//min dist = 1m  64
	int maxDist = 1000;
	float distance = abs((v-curPos).length());
	int volume = 64;

	if (distance > 1)
		volume -= (int)(64.0*distance/maxDist);
	// LOG("Distance: " << distance);

	_loadedFiles[file]->volume = sound_volume;

	if(distance <= 30)
		_loadedFiles[file]->volume = _loadedFiles[file]->volume;
	else if(distance < maxDist)
		_loadedFiles[file]->volume = _loadedFiles[file]->volume * ((float)maxDist / (distance * 3.5));
	else
		_loadedFiles[file]->volume = 0;

	if (volume > 0) 
	{
		// LOG("Volume: " << volume);
		int chan = Mix_PlayChannel(-1, _loadedFiles[file], loops);
	}

	//return Mix_PlayChannel(-1, _loadedFiles[file], loops);
}

void AudioManager::stopChannel(AudioChannel channel) {
	Mix_HaltChannel(channel);
}

void AudioManager::playDonk() {
	play2DSound(_donks[rand() % 4], 0);
}

void AudioManager::playStartSound() {
	play2DSound(_startSound, 0);
}

void AudioManager::playBALLZ() {
	play2DSound(_ballzSound, 0);
}

void AudioManager::playWeaponFire(Ogre::Vector3 v, uint32_t weapon_id, uint32_t sound_volume) {
	current_fire++;
	if(current_fire == SOUNDS_NUM_PER_WEAPON)
		current_fire = 0;
	play3DSound(_rifleFire[weapon_id][current_fire], 0, v, sound_volume);
}

void AudioManager::playBulletDirtCollision(Ogre::Vector3 v) {
	current_dust++;
	if(current_dust == SOUNDS_NUM_BULLET)
		current_dust = 0;
	play3DSound(_bulletDirtCollision[current_dust], 0, v);
}

void AudioManager::playWalkStep(Ogre::Vector3 v) {
	play3DSound(_footstep[RAND_RANGE(0, 7)], 0, v);
}

void AudioManager::playJetPack(Ogre::Vector3 v) {
	play3DSound(_jetpack, 0, v);
}

void AudioManager::playReload(Ogre::Vector3 v, uint32_t weapon_id) 
{
	play3DSound(_reloads[weapon_id], 0, v);
}

void AudioManager::playDeath(Ogre::Vector3 v) {
	play3DSound(_death, 0, v);
}

void AudioManager::playBlootSplat(Ogre::Vector3 v)
{
	current_blood++;
	if(current_blood == SOUNDS_NUM_BLOOD)
		current_blood = 0;
	play3DSound(_blood_splats[current_blood], 0, v);
}

void AudioManager::playBlasterCharge(Ogre::Vector3 v, uint32_t sound_volume)
{
	play3DSound(_blaster_charge, 0, v, sound_volume);
}

void AudioManager::playOutOfAmmo(Ogre::Vector3 v)
{
	play3DSound(_out_of_ammo, 0, v);
}

void AudioManager::playPickWeapon(Ogre::Vector3 v)
{
	play3DSound(_pick_weapon, 0, v);
}

void AudioManager::playPintoSpawn(Ogre::Vector3 v)
{
	play3DSound(_pinto_spawn, 0, v);
}

void AudioManager::playHairChange(Ogre::Vector3 v)
{
	play3DSound(_change_hair, 0, v);
}

void AudioManager::playPintoDie(Ogre::Vector3 v)
{
	play3DSound(_pinto_die, 0, v);
}

void musicDone()
{
	if(AudioManager::instance()->in_pinto)
	{
		Mix_HaltMusic();
		AudioManager::instance()->current_music = AudioManager::instance()->pinto_music;
		Mix_PlayMusic(AudioManager::instance()->current_music, 0);
	}
	else
	{
		Mix_HaltMusic();
		AudioManager::instance()->current_music =  AudioManager::instance()->normal_music[RAND_RANGE(0, MUSIC_NUM)];
		Mix_PlayMusic(AudioManager::instance()->current_music, 0);
	}
}
void AudioManager::startMusic()
{
	current_music = normal_music[RAND_RANGE(0, MUSIC_NUM)];
	Mix_PlayMusic(current_music, 0);
}
void AudioManager::stopMusic()
{
	Mix_HaltMusic();
}
void AudioManager::stopPinto()
{
	in_pinto = false;
	Mix_HaltMusic();
	current_music =  normal_music[RAND_RANGE(0, MUSIC_NUM)];
	Mix_PlayMusic(current_music, 0);
}
void AudioManager::startPinto()
{
	in_pinto = true;
	Mix_HaltMusic();
	current_music = pinto_music;
	Mix_PlayMusic(current_music, 0);
}