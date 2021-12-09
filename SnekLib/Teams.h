#pragma once

#include <tuple>
#include <string>

enum class Team
{
		GMolnarASporka,
		JFormanekVPetrov,
		MMarkoLHosek,
		MLabutRKrenek,
		PManMSourek,
		PNohejlMMatous,
		PSmrcekLSuk,
		TBarakMJarolimek,
		TGrunwaldDMikes,
		TJanakMKlima,
		TVahalikRSevcik,
		Dummy,
		Human
};

using TeamNames = std::pair<std::string, std::string>;

inline TeamNames GetTeamNames(Team team)
{
		switch (team) {
		case Team::GMolnarASporka: return { "gabor.molnar", "adam.sporka" };
		case Team::JFormanekVPetrov: return { "jiri.formanek", "vadim.petrov" };
		case Team::MMarkoLHosek: return { "matej.marko", "lukas.hosek" };
		case Team::MLabutRKrenek: return { "martin.labut", "radim.krenek" };
		case Team::PManMSourek: return { "petr.man", "martin.sourek" };
		case Team::PNohejlMMatous: return { "petr.nohejl", "martin.matous" };
		case Team::PSmrcekLSuk: return { "petr.smrcek","lubos.suk" };
		case Team::TBarakMJarolimek: return { "tomas.barak", "michal.jarolimek" };
		case Team::TGrunwaldDMikes:  return { "tomas.grunwald", "daniel.mikes" };
		case Team::TJanakMKlima: return { "tomas.janak", "martin.klima" };
		case Team::TVahalikRSevcik:  return { "tomas.vahalik", "radek.sevcik" };
		}
		return { "dummy", "dummy" };
}

inline std::string GetTeamName(Team team)
{
		auto t = GetTeamNames(team);
		return t.first + "_" + t.second; 
}

static inline std::vector<Team> AllTeams = {
				Team::GMolnarASporka,
				Team::JFormanekVPetrov,
				Team::MMarkoLHosek,
				Team::MLabutRKrenek,
				Team::PManMSourek,
				Team::PNohejlMMatous,
				Team::PSmrcekLSuk,
				Team::TBarakMJarolimek,
				Team::TGrunwaldDMikes,
				Team::TJanakMKlima,
				Team::TVahalikRSevcik
};

