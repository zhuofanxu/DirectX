//-------------------------------------------------------------------------------------
//File name: ParticleSystem
//Desc:粒子系统抽象类，用来操作粒子对象，完成粒子的生成，渲染，更新，消亡以及再生成的过程
//-------------------------------------------------------------------------------------
#pragma once
#include "Particle.h"
#ifndef _ParticleSystem_H
#define _ParticleSystem_H
class CParticleSystem
{
public:
  CParticleSystem(void);
	virtual ~CParticleSystem(void);

	virtual InitParticle(void)=0;                    //粒子初始化
	virtual RenderParticle(void)=0;                 //（绘制)渲染粒子
	virtual UpdateParticle(FLOAT ftime)=0;          //粒子更新
	virtual AddParticle(void)=0;                    //添加粒子到粒子系统
    virtual ResetParticle(CParticle*)=0;                  //重置粒子

};
#endif  //_ParticleSystem_H
