//------------------------------------------------------------------------
//File Name:Particle.h
//Desc     :粒子类
//------------------------------------------------------------------------
#pragma once
#include <d3dx9.h>
#ifndef _Particle_h_
#define _Particle_h_
class CParticle
{
public:
  CParticle(void);
	~CParticle(void);
	/*inline bool GetisLive(VOID){return m_isLive;}
	inline FLOAT GetlifeTiem(VOID){return m_lifeTime;}
	inline FLOAT Getage(VOID){return m_age;}
	inline D3DXVECTOR3 Getposition(VOID){return m_position;}
	inline D3DXVECTOR3 GetcurrVolecity(VOID){return m_currVolecity;}*/
public:
	bool m_isLive;                    //粒子存活标识
	FLOAT m_lifeTime;                 //粒子的生命周期(寿命)

	FLOAT m_age;                     //粒子持续生存的时间，如果m_age>m_lifeTime表示粒子死亡

	D3DXVECTOR3 m_position;          //粒子当前的位置
	
	D3DXVECTOR3 m_currVolecity;     //粒子当前速率

	DWORD m_color;                  //粒子颜色
	D3DXVECTOR3 m_initalVolecity;    //初始速率
	//ParticleConfig

};
#endif

