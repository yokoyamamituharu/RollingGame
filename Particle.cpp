#include "Particle.h"
#include "Useful.h"

Particle* Particle::GetIns()
{
	static Particle ins;
	return &ins;
}

void Particle::Add(ParticleTubu* tubu)
{
	particles.emplace_front();
	ParticleTubu& p = particles.front();
	p = *tubu;

}

void Particle::Initialize()
{
}

void Particle::Update()
{
	//寿命が尽きたパーティクルを全削除
	particles.remove_if(
		[](ParticleTubu& x)
		{
			return x.frame >= x.end_frame;
		}
	);


	//全パーティクル更新
	for (ParticleTubu& particle : particles)
	{
		//経過フレーム数をカウント
		particle.frame++;
		//速度による移動
		particle.position = particle.position + particle.velocity;
		//進行度0～1の範囲に換算
		float f = (float)particle.end_frame / particle.frame;
		if (particle.obj == nullptr) {
			continue;
		}
		particle.obj->SetPosition(particle.position);
		if (particle.startScale != 0) {
			float sscale = (particle.endScale - particle.scale) / f;
			sscale = sscale + particle.scale;
			particle.obj->SetScale({ sscale,sscale,sscale });
		}
		particle.obj->Update();
	}
}

void Particle::Draw()
{
	for (ParticleTubu& particle : particles) {
		if (particle.obj == nullptr) {
			continue;
		}
		particle.obj->Draw();
	}
}
