/* let personal2PageData = sheetData => { */
/*   print_endline("-----------------------Personal 2-------------"); */
/*   let nationalityData = List.assoc("NATIONALITY", sheetData); */
/*   let nationality = */
/*     switch (nationalityData) { */
/*     | Some(nationality) => nationality */
/*     | None => raise(Failure("nationality cannot be null")) */
/*     }; */
/*   /\*TODO: NEED to Construct list here*\/ */
/*   let otherNationalityData = List.assoc("OTHER_NATIONALITY", sheetData); */
/*   let otherPassportData = List.assoc("OTHER_PASSPORT_NUM", sheetData); */
/*   let otherNationality = */
/*     switch (otherNationalityData) { */
/*     | Some(nationality) => */
/*       switch (otherPassportData) { */
/*       | Some(passportNum) => */
/*         Some([ */
/*           ( */
/*             {nationality, passportNum: Some(passportNum)}: DsDataTypes.nationalityInfo */
/*           ), */
/*         ]) */
/*       | None => */
/*         Some([ */
/*           ({nationality, passportNum: None}: DsDataTypes.nationalityInfo), */
/*         ]) */
/*       } */
/*     | None => None */
/*     };  */
/*   let permanentResidentData = List.assoc("PERMANAT_RESIDENT", sheetData); */
/*   let permanentResident = */
/*     switch (otherNationalityData) { */
/*     | Some(nationality) => */
/*       switch (otherPassportData) { */
/*       | Some(passportNum) => */
/*         Some([ */
/*         ( */
/*           {nationality, passportNum: Some(passportNum)}: DsDataTypes.nationalityInfo */
/*         ), */
/*       ]) */
/*       | None => */
/*         Some([ */
/*         ({nationality, passportNum: None}: DsDataTypes.nationalityInfo), */
/*       ]) */
/*       } */
/*     | None => None */
/*     }; */
/*   let personal2: DsDataTypes.personal2 = { */
/*     nationality, */
/*     otherNationality, */
/*     permanentResident, */
/*     nationalId, */
/*     ssn, */
/*     taxId, */
/*   }; */
/*   (); */
/* }; */
