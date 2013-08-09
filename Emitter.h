//-------------------------------------------------------------------------------------
//File name：Emitter.h
//Desc:粒子发射器类，继承自粒子系统类，实现其所有的纯虚函数
//-------------------------------------------------------------------------------------
#pragma once
#include "particlesystem.h"
#include <vector>
#ifndef  _Emitter_h_
#define _Emitter_h_
class CEmitter :public CParticleSystem
{
  
public:
	CEmitter(LPDIRECT3DDEVICE9 device);
    ~CEmitter(void);

	void InitParticle(void);                    //粒子初始化
	void RenderParticle(void);                 //（绘制)渲染粒子
	void UpdateParticle(FLOAT ftime);          //粒子更新
	void AddParticle(void);                    //添加粒子到粒子系统
	void ResetParticle(CParticle*);             //重置粒子

private:
	std::vector<CParticle> v_bufferPar;         //粒子容器(动态数组)
	std::vector<CParticle>::iterator itor_Par;  //粒子容器迭代器
    D3DXVECTOR3 m_position;                     //发射器位置

	DWORD m_particleColor;
	int   m_numParticle;                        //发射器最大的粒子数

};
#endif



