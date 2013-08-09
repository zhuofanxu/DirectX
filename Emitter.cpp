#include "Emitter.h"

CEmitter::CEmitter(LPDIRECT3DDEVICE9 device)
{

}

CEmitter::~CEmitter(void)
{
}

void CEmitter::AddParticle(void)
{
  CParticle particle;
	ResetParticle(&particle);
	v_bufferPar.push_back(particle);
}
void CEmitter::ResetParticle(CParticle *particle)
{
	particle->m_isLive=true;                          //激活粒子
	particle->m_color=m_particleColor;
	particle->m_position=m_position;                 //每个粒子的初始位置由发射器位置决定
	//particle->m_lifeTime=                           //生命周期随机值(0-1)
	D3DXVECTOR3 max=D3DXVECTOR3(1.0f,1.0f,1.0f);      //定义速度区间最大值
	D3DXVECTOR3 min=D3DXVECTOR3(-1.0f,-1.0f,-1.0f);   //定义速度区间最小值
    particle->m_currVolecity=                         //速度随机值[(-1,-1,-1),(1,1,1)]

}
void CEmitter::UpdateParticle(FLOAT ftime)
{
	for (itor_Par=v_bufferPar.begin();itor_Par<v_bufferPar.end();++itor_Par)  //通过迭代器遍历容器内所有的粒子
	{
		if(itor_Par->m_isLive)                                              //只更新存活的粒子
		{
			itor_Par->m_age+=ftime;                                         //存活时间更新
            itor_Par->m_position=ftime*itor_Par->m_currVolecity*10.0;        //位置更新
            itor_Par->m_color=this->m_particleColor;                         //颜色更新
			if (itor_Par->m_age>itor_Par->m_lifeTime)                       // 死亡更新           
				itor_Par->m_isLive=false;
		}
		else
			ResetParticle((CParticle*)&(*itor_Par));
	}
	for (int i=0;i<m_numParticle;++i)                             //填充粒子容器
	{
		AddParticle();
	}

}
