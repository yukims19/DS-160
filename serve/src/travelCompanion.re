open WorkerCommon;
open DsDataTypes;
open CountryType;

let stringOfTravelCompanionPageData = (p6Data: DsDataTypes.travelCompanions) =>
  Printf.sprintf(
    "
  Travel Companion: %s",
    stringOfTravelCompanion(p6Data.companion),
  );

let travelCompanionPageData =
    (worksheetValues: worksheetValues)
    : travelCompanions => {
  companion:
    pluckCompanion(
      pluckTravelCompanionGroupName(worksheetValues),
      pluckTravelCompanionSurname(worksheetValues),
      pluckTravelCompanionGiveName(worksheetValues),
      pluckTravelCompanionRelationshipRaw(worksheetValues),
    ),
};
