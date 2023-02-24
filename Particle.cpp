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
	//�������s�����p�[�e�B�N����S�폜
	particles.remove_if(
		[](ParticleTubu& x)
		{
			return x.frame >= x.end_frame;
		}
	);


	//�S�p�[�e�B�N���X�V
	for (ParticleTubu& particle : particles)
	{
		//�o�߃t���[�������J�E���g
		particle.frame++;
		//���x�ɂ��ړ�
		particle.position = particle.position + particle.velocity;
		//�i�s�x0�`1�͈̔͂Ɋ��Z
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
