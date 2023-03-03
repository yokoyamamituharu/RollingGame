#include "Particle.h"
#include "Useful.h"
#include "safe_delete.h"


void Particle::Add(std::unique_ptr<ParticleTubu> tubu)
{
	//particles.emplace_front();
	//ParticleTubu& p = particles.front();
	particles.push_back(std::move(tubu));
	//p = *tubu;

}

void Particle::Initialize()
{
}

void Particle::Update()
{
	//�������s�����p�[�e�B�N����S�폜
	particles.remove_if([](std::unique_ptr<ParticleTubu>& x) {return x->frame >= x->end_frame; });


	//�S�p�[�e�B�N���X�V
	for (std::unique_ptr<ParticleTubu>& particle : particles)
	{
		//�o�߃t���[�������J�E���g
		particle->frame++;
		//���x�ɂ��ړ�
		particle->position = particle->position + particle->velocity;
		//�i�s�x0�`1�͈̔͂Ɋ��Z
		float f = (float)particle->end_frame / particle->frame;
		if (particle->obj == nullptr) {
			continue;
		}
		particle->obj->SetPosition(particle->position);
		if (particle->startScale != 0) {
			float sscale = (particle->endScale - particle->scale) / f;
			sscale = sscale + particle->scale;
			particle->obj->SetScale({ sscale,sscale,sscale });
		}
		particle->obj->Update();
	}
	int a = 0;
}

void Particle::Draw()
{
	for (std::unique_ptr<ParticleTubu>& particle : particles) {
		if (particle->obj == nullptr) {
			continue;
		}
		particle->obj->Draw();
	}
}

