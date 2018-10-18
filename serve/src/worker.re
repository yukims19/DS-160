/* TODO: -Get googleSheet id from job
            - fetch OneGraph query of googleSheet
            - talk to Ds-website
            - complete the task
   */
open Async;
open Cohttp;
open Cohttp_async;

let getSheetDataQuery = sheetId =>
  "{\"query\":\"query {\\n  google {\\n    sheets {\\n      sheet(\\n        includeGridData: true\\n        id: \\\""
  ++ sheetId
  ++ "\\\"\\n      ) {\\n        namedRanges {\\n          name\\n          namedRangeId\\n        }\\n        spreadsheetId\\n        spreadsheetUrl\\n        properties {\\n          title\\n          autoRecalc\\n        }\\n        sheets {\\n          data {\\n            rowData {\\n              values {\\n                formattedValue\\n              }\\n            }\\n            startRow\\n          }\\n        }\\n      }\\n    }\\n  }\\n}\\n\",\"variables\":null}";

let oneGraphQueryHeaderList = [
  ("Authentication", "Bearer EllXOnjB6_gB3Aq9MQ55c1u4Ymz4AFyHfx9jEBuhzsQ"),
  ("Accept", "application/json"),
];

let rowDataOfOneGraphQuery = resultJson =>
  Yojson.Basic.Util.(
    resultJson
    |> member("data")
    |> member("google")
    |> member("sheets")
    |> member("sheet")
    |> member("sheets")
    |> to_list
    |> List.hd
    |> member("data")
    |> to_list
    |> List.hd
    |> member("rowData")
    |> to_list
  );

type worksheetValues = list((string, list(string)));

let getSheetData = sheetId => {
  print_endline("-----------------------here is the worker-------------");
  let headerList = oneGraphQueryHeaderList;
  let headers = Cohttp.Header.of_list(headerList);
  let body = Cohttp_async.Body.of_string(getSheetDataQuery(sheetId));

  Cohttp_async.Client.post(
    Uri.of_string(
      "https://serve.onegraph.com/dynamic?app_id=d886069e-e695-4e76-92a6-042501f18ce6",
    ),
    ~headers,
    ~body,
  )
  >>= (
    ((resp, body)) => {
      let code = resp |> Response.status |> Code.code_of_status;
      print_endline("Response code:" ++ string_of_int(code));
      let headerList = resp |> Response.headers |> Header.to_frames;
      print_endline("Headers:");
      List.iter(head => print_endline(head), headerList);

      body
      |> Cohttp_async.Body.to_string
      >>= (
        body => {
          open Yojson.Basic.Util;
          let bodyJson = Yojson.Basic.from_string(body);
          let allRows = rowDataOfOneGraphQuery(bodyJson);

          let isEmptyRow = row => {
            let values =
              row
              |> member("values")
              |> to_list
              |> List.map(member("formattedValue"));

            let nonEmptyCells =
              values
              |> List.filter(cell =>
                   switch (cell) {
                   | `String(_) => true
                   | `Null
                   | _ => false
                   }
                 );
            List.length(nonEmptyCells) == 0;
          };

          let rows = List.filter(row => ! isEmptyRow(row), allRows);

          let rowToKeyValues = row => {
            let cells =
              row
              |> member("values")
              |> to_list
              |> List.map(member("formattedValue"))
              |> List.map(to_string_option);

            let maybeKey = List.hd(cells);
            let key =
              switch (maybeKey) {
              | None =>
                raise(Failure("Missing label for field in spreadsheet"))
              | Some(key) => String.(lowercase_ascii(key) |> trim)
              };

            let values =
              Core.List.filter_map(~f=cell => cell, List.tl(cells));

            (key, values);
          };

          let worksheetValues = List.map(rowToKeyValues, rows);

          Deferred.return(worksheetValues);
        }
      );
    }
  );
};

let constructPageData = sheetData =>
  sheetData
  >>= (
    sheetData => {
      let p1Data = DsPersonal1.personal1PageData(sheetData);
      OneLog.infof(
        "Got P1Data: %s",
        DsPersonal1.stringOfPersonal1PageData(p1Data),
      );
      let p2Data = DsPersonal2.personal2PageData(sheetData);
      OneLog.infof(
        "Got P2Data: %s",
        DsPersonal2.stringOfPersonal2PageData(p2Data),
      );
      let p3Data = AddressAndPhone.addressAndPhonePageData(sheetData);
      OneLog.infof(
        "Got P3Data: %s",
        AddressAndPhone.stringOfAddressAndPhonePageData(p3Data),
      );
      let p4Data = Passport.passportPageData(sheetData);
      OneLog.infof(
        "Got P4Data: %s",
        Passport.stringOfPassportPageData(p4Data),
      );
      /*TODO: Fix type for p5*/
      let p5Data = Travel.travelPageData(sheetData);
      OneLog.infof("Got P5Data: %s", Travel.stringOfTravelPageData(p5Data));
      let p6Data = TravelCompanion.travelCompanionPageData(sheetData);
      OneLog.infof(
        "Got P6Data: %s",
        TravelCompanion.stringOfTravelCompanionPageData(p6Data),
      );
      let p7Data = PreviousUSTravel.previousUSTravelPageData(sheetData);
      OneLog.infof(
        "Got P7Data: %s",
        PreviousUSTravel.stringOfPreviousUSTravelPageData(p7Data),
      );
      let p8Data = UsContact.usContactPageData(sheetData);
      OneLog.infof(
        "Got P8Data: %s",
        UsContact.stringOfUSContactPageData(p8Data),
      );
      Deferred.return();
    }
  );

let init = (_spec, ()) => {
  let _sheetId1 = "19CNUappgAuMqKAOHD0_1CusGHTlRMH7V4DbC03DUtrE";
  let sheetId2 = "1hR-GbybIfuEWOU76jZA31nBCodLDfZ1LHGCiXBMIQp8";
  let _sheetId3 = "1FM3H6mS1uIYzgvzMERVGLy2ncvUGx82uHT2vof5J1nY";
  let summarySheetData = getSheetData(sheetId2);
  let pageData = constructPageData(summarySheetData);
  pageData;
  /* Deferred.return();*/
};
