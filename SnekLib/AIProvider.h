#pragma once

#include "SnekAI_GMolnar_ASporka.h"
#include "SnekAI_JFormanek_VPetrov.h"
#include "SnekAI_MMarko_LHosek.h"
#include "SnekAI_MLabut_RKrenek.h"
#include "SnekAI_PMan_MSourek.h"
#include "SnekAI_PNohejl_MMatous.h"
#include "SnekAI_PSmrcek_LSuk.h"
#include "SnekAI_TBarak_MJarolimek.h"
#include "SnekAI_TGrunwald_DMikes.h"
#include "SnekAI_TJanak_MKlima.h"
#include "SnekAI_TVahalik_RSevcik.h"

#include "SnekAI_Human.h"
#include "SnekAI_Dummy.h"
#include "Teams.h"

inline SnekAI* GetAI(Team team) 
{
		switch (team) {
		case Team::GMolnarASporka: return new SnekAI_GMolnar_ASporka();
		case Team::JFormanekVPetrov: return new SnekAI_JFormanek_VPetrov();
		case Team::MMarkoLHosek: return new SnekAI_MMarko_LHosek();
		case Team::MLabutRKrenek: return new SnekAI_MLabut_RKrenek();
		case Team::PManMSourek: return new SnekAI_PMan_MSourek();
		case Team::PNohejlMMatous : return new SnekAI_PNohejl_MMatous();
		case Team::PSmrcekLSuk: return new SnekAI_PSmrcek_LSuk();
		case Team::TBarakMJarolimek: return new SnekAI_TBarak_MJarolimek();
		case Team::TGrunwaldDMikes: return new SnekAI_TGrunwald_DMikes();
		case Team::TJanakMKlima: return new SnekAI_TJanak_MKlima();
		case Team::TVahalikRSevcik: return new SnekAI_TVahalik_RSevcik();
		case Team::Human: return new SnekAI_Human();
		}
		return new SnekAI_Dummy();
}

