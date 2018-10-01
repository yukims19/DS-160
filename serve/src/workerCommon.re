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

let nonOptionString = (key, value) =>
  switch (value) {
  | Some(realValue) => realValue
  | None => raise(Failure(Printf.sprintf("%s cannot be empty", key)))
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
  surname: nonOptionString("SURNAME", surname),
  givenName: nonOptionString("GIVEN_NAME", givenName),
};

let pluckP1DOBDay = makePluckerOne("DATE_OF_BIRTHDay");
let pluckP1DOBMonth = makePluckerOne("DATE_OF_BIRTHMonth");
let pluckP1DOBYear = makePluckerOne("DATE_OF_BIRTHYear");

let pluckDate = (day, month, year) : DsDataTypes.date => {
  day: nonOptionString("day", day),
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
  year: nonOptionString("year", year),
};

let pluckOptionDate = (day, month, year) =>
  switch (day) {
  | Some(d) =>
    switch (month) {
    | Some(m) =>
      switch (year) {
      | Some(y) => Some(pluckDate(d, m, y))
      | None =>
        raise(
          Failure(
            "Date, Month, Year should all have some value or all be empty",
          ),
        )
      }
    | None =>
      raise(
        Failure(
          "Date, Month, Year should all have some value or all be empty",
        ),
      )
    }
  | None => None
  };

let pluckP1POBCity = makePluckerOne("PLACE_OF_BIRTH_CITY");
let pluckP1POBState = makePluckerOne("PLACE_OF_BIRTH_STATE_PROVINCE");
let pluckP1POBCountry = makePluckerOne("PLACE_OF_BIRTH_CNTRY");

let shortAddress = (city, state, country) : DsDataTypes.shortAddress => {
  city: nonOptionString("CITY", city),
  state,
  country:
    switch (country) {
    | Some(country) =>
      CountryType.countryTypeOfFullCountryName(
        String.capitalize_ascii(country),
      )
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
let nation = nation =>
  switch (nation) {
  | Some(nation) =>
    String.map(a => Char.uppercase_ascii(a), nation)
    |> CountryType.countryTypeOfFullCountryName

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
          let nationality =
            String.map(a => Char.uppercase_ascii(a), nation)
            |> CountryType.countryTypeOfFullCountryName;

          ({nationality, passportNum}: DsDataTypes.nationalityInfo);
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
let optionListCountry = countries =>
  switch (countries) {
  | Some(countries) =>
    Some(
      List.map(
        country =>
          CountryType.countryTypeOfFullCountryName(
            String.capitalize_ascii(country),
          ),
        countries,
      ),
    )
  | None => None
  };
let pluckNationalId = makePluckerOne("NATIONAL_ID");
let pluckSNN = makePluckerOne("SSN");
let pluckTaxId = makePluckerOne("TAX_ID");

let pluckHomeStreet1 = makePluckerOne("HOME_ADDR_LN1");
let pluckHomeStreet2 = makePluckerOne("HOME_ADDR_LN2");
let pluckHomeCity = makePluckerOne("HOME_ADDR_CITY");
let pluckHomeState = makePluckerOne("HOME_ADDR_STATE");
let pluckHomeZipCode = makePluckerOne("HOME_ADDR_POSTAL_CD");
let pluckHomeCountry = makePluckerOne("HOME_ADDR_COUNTRY");
let address =
    (streetL1, streetL2, city, state, zipCode, country)
    : DsDataTypes.address => {
  streetL1: nonOptionString("StreetL1", streetL1),
  streetL2,
  city: nonOptionString("CITY", city),
  state,
  zipCode,
  country: nation(country),
};

let pluckMailingStreet1 = makePluckerOne("MAILING_ADDR_LN1");
let pluckMailingStreet2 = makePluckerOne("MAILING_ADDR_LN2");
let pluckMailingCity = makePluckerOne("MAILING_ADDR_CITY");
let pluckMailingState = makePluckerOne("MAILING_ADDR_STATE");
let pluckMailingZipCode = makePluckerOne("MAILING_ADDR_POSTAL_CD");
let pluckMailingCountry = makePluckerOne("MAILING_ADDR_COUNTRY");
let optionAddress = (streetL1, streetL2, city, state, zipCode, country) =>
  switch (streetL1) {
  | Some(_street) =>
    Some(
      {
        streetL1: nonOptionString("StreetL1", streetL1),
        streetL2,
        city: nonOptionString("CITY", city),
        state,
        zipCode,
        country: nation(country),
      }: DsDataTypes.address,
    )
  | None => None
  };

let pluckPrimaryPhone = makePluckerOne("HOME_TEL");
let pluckSecPhone = makePluckerOne("MOBILE_TEL");
let pluckWorkPhone = makePluckerOne("HOME_TEL");
let pluckEmail = makePluckerOne("EMAIL");

let pluckPassportRaw = makePluckerOne("PASSPORT_TYPE");
let pluckPassport = worksheetValues : DsDataTypes.passType =>
  switch (pluckPassportRaw(worksheetValues)) {
  | Some(passType) =>
    switch (passType) {
    | "Regular" => Regular
    | "Official" => Official
    | "Diplomatic" => Diplomatic
    | "Laissez" => Laissez
    | "Other" => Other
    | _value =>
      raise(
        Failure(
          "Invalide Passport type: "
          ++ _value
          ++ ". Please enter Regular, Official, Diplomatic, Laissez or Other",
        ),
      )
    }
  | None => raise(Failure("PASSPORT_TYPE cannot be empty"))
  };

let pluckPassportNum = makePluckerOne("PASSPORT_NUM");
let pluckPassportBookNum = makePluckerOne("PASSPORT_BOOK_NUM");
let pluckPassportIssuedCountry = makePluckerOne("PASSPORT_ISSUED_COUNTRY");
let pluckPassportIssuedCity = makePluckerOne("PASSPORT_ISSUED_IN_CITY");
let pluckPassportIssuedState = makePluckerOne("PASSPORT_ISSUED_IN_STATE");
let pluckPassportIssuedCountry = makePluckerOne("PASSPORT_ISSUED_IN_COUNTRY");

let pluckPassportIssuedDay = makePluckerOne("PASSPORT_ISSUED_DAY");
let pluckPassportIssuedMonth = makePluckerOne("PASSPORT_ISSUED_MONTH");
let pluckPassportIssuedYear = makePluckerOne("PASSPORT_ISSUED_YEAR");

let pluckPassportExprieDay = makePluckerOne("PASSPORT_EXPIRE_DAY");
let pluckPassportExprieMonth = makePluckerOne("PASSPORT_EXPIRE_MONTH");
let pluckPassportExprieYear = makePluckerOne("PASSPORT_EXPIRE_YEAR");

let pluckLostPassportNum = makePluckerMulti("LOST_PASSPORT_NUM");
let pluckLostPassportIssuedCountry =
  makePluckerMulti("LOST_PASSPORT_ISSUED_COUNTRY");
let pluckLostPasssportExplain = makePluckerMulti("LOST_PASSPORT_EXPLAIN");

let optionListOfLostPassInfo =
    (passNum, countryIssue, explain)
    : option(list(DsDataTypes.lostPassInfo)) =>
  switch (passNum, countryIssue, explain) {
  | (None, None, None) => None
  | (Some(num), Some(country), Some(explain)) =>
    if (List.length(num) == List.length(country)
        && List.length(country) == List.length(explain)) {
      let numAndCountryList =
        List.map2(
          (num, country) => {
            let passportNum =
              switch (num) {
              | "N/A" => None
              | _ => Some(num)
              };
            let nation =
              String.map(a => Char.uppercase_ascii(a), country)
              |> CountryType.countryTypeOfFullCountryName;

            ({passportNum, nationality: nation}: DsDataTypes.nationalityInfo);
          },
          num,
          country,
        );
      let numAndCountryAndExplainList =
        List.map2(
          (numAndCountry: DsDataTypes.nationalityInfo, explain) => (
            {
              passNum: numAndCountry.passportNum,
              countryIssue: numAndCountry.nationality,
              explain,
            }: DsDataTypes.lostPassInfo
          ),
          numAndCountryList,
          explain,
        );
      Some(numAndCountryAndExplainList);
    } else {
      raise(
        Failure(
          "Lost passport number, issued country and explain needs to be same length",
        ),
      );
    }
  | _ =>
    raise(
      Failure(
        "OTHER_NATIONALITY and OTHER_PASSPORT_NUM must either both be absent or both be present. Please enter N/A for unknow passport number",
      ),
    )
  };
