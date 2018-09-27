type worksheetValues = list((string, list(string)));

let makePluckerOne = (key, worksheetValues: worksheetValues) => {
  let values =
    try (List.assoc(String.lowercase_ascii(key), worksheetValues)) {
    | Not_found =>
      raise(
        Failure(
          Printf.sprintf(
            "Could not find key in worksheet for pluckerOne: %s",
            key,
          ),
        ),
      )
    };
  switch (values) {
  | [] => None
  | [value] => Some(value)
  | values =>
    raise(
      Failure(
        Printf.sprintf(
          "More than one %s found: [%s]",
          key,
          String.concat(",", values),
        ),
      ),
    )
  };
};

let makePluckerMulti = (key, worksheetValues: worksheetValues) => {
  let values =
    try (List.assoc(String.lowercase_ascii(key), worksheetValues)) {
    | Not_found =>
      raise(
        Failure(
          Printf.sprintf(
            "Could not find key in worksheet for pluckerMulti: %s",
            key,
          ),
        ),
      )
    };
  switch (values) {
  | [] => None
  | values => Some(values)
  };
};

let pluckGivenName = makePluckerOne("GIVEN_NAME");
let pluckSurname = makePluckerOne("SURNAME");
let pluckGenderRaw = makePluckerOne("GENDER");
let pluckMaritalStatusRaw = makePluckerOne("MARITAL_STATUS");

let pluckMaritalStatus = worksheetValues : DsDataTypes.marital =>
  switch (pluckMaritalStatusRaw(worksheetValues)) {
  | Some(marital) =>
    switch (marital) {
    | "Married" => Married
    | "Single" => Single
    | "Common" => Common
    | "Civil" => Civil
    | "Widowed" => Widowed
    | "Divorced" => Divorced
    | "Seperate" => Seperate
    | "Other" => Other
    | _ =>
      raise(
        Failure(
          "Invalid value for MARITAL_STATUS. Please enter Married, Single, Common, Civil, Widowed, Divorced, Seperate or Other",
        ),
      )
    }
  | None => raise(Failure("MARITAL_STATUS cannot be null"))
  };

let pluckGender = worksheetValues =>
  DsDataTypes.(
    switch (pluckGenderRaw(worksheetValues)) {
    | Some(gender) =>
      switch (gender) {
      | "F" => Female
      | "M" => Male
      | "FEMALE" => Female
      | "MALE" => Male
      | other =>
        raise(
          Failure(
            Printf.sprintf(
              "GENDER value '%s' is not valid for our system. Please enter 'F' or 'M'",
              other,
            ),
          ),
        )
      }
    | None => raise(Failure("Gender cannot be null"))
    }
  );

let fullName = (surname, givenName) : DsDataTypes.name => {
  surname:
    switch (surname) {
    | Some(surname) => surname
    | None => raise(Failure("Surname cannot be null"))
    },
  givenName:
    switch (givenName) {
    | Some(givenName) => givenName
    | None => raise(Failure("Given_name cannot be null"))
    },
};

let pluckP1DOBDay = makePluckerOne("DATE_OF_BIRTHDay");
let pluckP1DOBMonth = makePluckerOne("DATE_OF_BIRTHMonth");
let pluckP1DOBYear = makePluckerOne("DATE_OF_BIRTHYear");

let pluckDate = (day, month, year) : DsDataTypes.date => {
  day:
    switch (day) {
    | Some(day) => day
    | None => raise(Failure("day cannot be null"))
    },
  month:
    switch (month) {
    | Some(month) =>
      switch (month) {
      | "Jan" => Jan
      | "Feb" => Feb
      | "Mar" => Mar
      | "Apr" => Apr
      | "May" => May
      | "Jun" => Jun
      | "Jul" => Jul
      | "Aug" => Aug
      | "Sep" => Sep
      | "Oct" => Oct
      | "Nov" => Nov
      | "Dec" => Dec
      /* XXX: Might not be a good idea, maybe force the user to use the above
         format instead */
      | "1" => Jan
      | "2" => Feb
      | "3" => Mar
      | "4" => Apr
      | "5" => May
      | "6" => Jun
      | "7" => Jul
      | "8" => Aug
      | "9" => Sep
      | "10" => Oct
      | "11" => Nov
      | "12" => Dec
      | _ =>
        raise(
          Failure(
            "Invalid value for month. Please enter in format Jan, Feb, Mar ....",
          ),
        )
      }
    | None => raise(Failure("Surname cannot be null"))
    },
  year:
    switch (year) {
    | Some(year) => year
    | None => raise(Failure("year cannot be null"))
    },
};

let pluckP1POBCity = makePluckerOne("PLACE_OF_BIRTH_CITY");
let pluckP1POBState = makePluckerOne("PLACE_OF_BIRTH_STATE_PROVINCE");
let pluckP1POBCountry = makePluckerOne("PLACE_OF_BIRTH_CNTRY");

let pluckPlace = (city, state, country) : DsDataTypes.shortAddress => {
  city:
    switch (city) {
    | Some(city) => city
    | None => raise(Failure("CITY cannot be null"))
    },
  state,
  country:
    switch (country) {
    | Some(country) => country
    | None => raise(Failure("COUNTRY cannot be null"))
    },
};

let pluckNativeFullName = makePluckerOne("FULL_NAME_NATIVE");
let pluckTelecodeSurname = makePluckerOne("TELECODE_SURNAME");
let pluckTelecodeGivenName = makePluckerOne("TELECODE_GIVEN_NAME");

let optionFullName = (surname, givenName) =>
  switch (surname) {
  | Some(surname) =>
    switch (givenName) {
    | Some(givenName) => Some({surname, givenName}: DsDataTypes.name)
    | None => None
    }
  | None => None
  };

let pluckAliasSurname = makePluckerMulti("ALIAS_SURNAME");
let pluckAliasGivenName = makePluckerMulti("ALIAS_GIVEN_NAME");

/*TODO: NEED to Construct list here*/
let optionListFullNames = (aliasSurnames, aliasGivenNames) =>
  switch (aliasGivenNames, aliasSurnames) {
  | (None, None) => None
  | (Some(givenNames), Some(surnames)) =>
    Some(
      List.map2(
        (givenName, surname) => ({surname, givenName}: DsDataTypes.name),
        givenNames,
        surnames,
      ),
    )
  | (Some(_), None)
  | (None, Some(_)) =>
    raise(
      Failure(
        "ALIAS_SURNAME and ALIAS_GIVEN_NAME must either both be absent or both be present",
      ),
    )
  };

let pluckNationality = makePluckerOne("NATIONALITY");
let nationality = nation =>
  switch (nation) {
  | Some(nation) => nation
  | None => raise(Failure("Nationality cannot be null"))
  };

let pluckOtherNationality = makePluckerMulti("OTHER_NATIONALITY");
let pluckOtherPassportNum = makePluckerMulti("OTHER_PASSPORT_NUM");
let optionListNationalityInfo = (nation, passport) =>
  switch (nation, passport) {
  | (None, None) => None
  | (Some(nation), Some(passport)) =>
    Some(
      List.map2(
        (nation, passport) => {
          let passportNum =
            switch (passport) {
            | "N/A" => None
            | _ => Some(passport)
            };

          ({nationality: nation, passportNum}: DsDataTypes.nationalityInfo);
        },
        nation,
        passport,
      ),
    )
  | (Some(_), None)
  | (None, Some(_)) =>
    raise(
      Failure(
        "OTHER_NATIONALITY and OTHER_PASSPORT_NUM must either both be absent or both be present. Please enter N/A for unknow passport number",
      ),
    )
  };
let pluckPermanentResident = makePluckerMulti("PERMANENT_RESIDENT");
let pluckNationalId = makePluckerOne("NATIONAL_ID");
let pluckSNN = makePluckerOne("SSN");
let pluckTaxId = makePluckerOne("TAX_ID");
