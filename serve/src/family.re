open WorkerCommon;
open DsDataTypes;
open CountryType;

let stringOfFamilyPageData = (p9Data: DsDataTypes.family) =>
  Printf.sprintf(
    "
      Father: %s,
      Mother:%s,
      Relatives :%s,",
    stringOfFamilyMember(p9Data.father),
    stringOfFamilyMember(p9Data.mother),
    stringOfRelativeMember(p9Data.relative),
  );

let familyPageData = (worksheetValues: worksheetValues) : DsDataTypes.family => {
  father:
    pluckFamilyMember(
      pluckFatherSurname(worksheetValues),
      pluckFatherGiveName(worksheetValues),
      pluckFatherDOBDate(worksheetValues),
      pluckFatherDOBMonth(worksheetValues),
      pluckFatherDOBYear(worksheetValues),
      pluckFatherStayStatus(worksheetValues),
    ),
  mother:
    pluckFamilyMember(
      pluckMotherSurname(worksheetValues),
      pluckMotherGiveName(worksheetValues),
      pluckMotherDOBDate(worksheetValues),
      pluckMotherDOBMonth(worksheetValues),
      pluckMotherDOBYear(worksheetValues),
      pluckMotherStayStatus(worksheetValues),
    ),
  relative:
    pluckOptionListRelativeMember(
      pluckRelativeSurname(worksheetValues),
      pluckRelativeGiveName(worksheetValues),
      pluckRelativeRelationshipRaw(worksheetValues),
      pluckRelativeStayStatus(worksheetValues),
    ),
};
