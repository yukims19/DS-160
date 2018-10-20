open WorkerCommon;
open DsDataTypes;
open CountryType;

let stringOfAdditionalWETPageData = (p12Data: DsDataTypes.additionalWET) =>
  Printf.sprintf(
    "
     Tribe: %s,
     Lanuage: %s
     Travle: %s,
     Charity: %s,
     SpecialSkill: %s,
     Military: %s,
     Rebel: %s",
    stringOfOptionString("Tribe", p12Data.tribe),
    p12Data.language |> List.fold_left((a, b) => a ++ "/" ++ b, ""),
    stringOfOptionListCountry(p12Data.travel),
    stringOfOptionListString(p12Data.charity),
    stringOfOptionString("Special Skill", p12Data.specialSkill),
    "military",
    stringOfOptionString("Rebel", p12Data.rebel),
  );

let additionalWETPageData =
    (worksheetValues: worksheetValues)
    : DsDataTypes.additionalWET => {
  tribe: pluckAdditionalTribe(worksheetValues),
  language:
    nonOptionList(
      "Additional Language",
      pluckAdditionalLanguate(worksheetValues),
    ),
  travel: optionListCountry(pluckAdditionalTravel(worksheetValues)),
  charity: pluckAdditionalCharity(worksheetValues),
  specialSkill: pluckAdditionalSpecialSkill(worksheetValues),
  military:
    pluckAdditionalMilitary(
      optionListCountry(pluckAdditionalMilitaryCounty(worksheetValues)),
      pluckAdditionalMilitaryBranch(worksheetValues),
      pluckAdditionalMilitaryRank(worksheetValues),
      pluckAdditionalMilitarySpeciality(worksheetValues),
      pluckAdditionalMilitaryStartDay(worksheetValues),
      pluckAdditionalMilitaryStartMonth(worksheetValues),
      pluckAdditionalMilitaryStartYear(worksheetValues),
      pluckAdditionalMilitaryEndDay(worksheetValues),
      pluckAdditionalMilitaryEndMonth(worksheetValues),
      pluckAdditionalMilitaryEndYear(worksheetValues),
    ),
  rebel: pluckAdditonalRebelExpl(worksheetValues),
};
