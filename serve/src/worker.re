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
  ("Authentication", "Bearer tQhJQyN55Q3guJou50YSyeY3B8Rn_dXHbs7zglGl8nU"),
  ("Accept", "application/json"),
];

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
          let bodyJson = Yojson.Basic.from_string(body);
          let rowDataArray =
            Yojson.Basic.Util.(
              member("data", bodyJson)
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
              |> Array.of_list
            );

          let arrayOfRowValuesList =
            Array.map(
              rowValues => {
                let a =
                  Yojson.Basic.Util.(
                    rowValues |> member("values") |> to_list
                  );
                a;
              },
              rowDataArray,
            );
          let keyAndValueList =
            Array.map(
              value => {
                let formattedValueArray =
                  List.map(
                    a => {
                      let extractedValues =
                        Yojson.Basic.Util.(
                          a |> member("formattedValue") |> to_string
                        );
                      extractedValues;
                    },
                    value,
                  )
                  |> Array.of_list;

                if (Array.length(formattedValueArray) > 1) {
                  (formattedValueArray[0], Some(formattedValueArray[1]));
                } else {
                  (formattedValueArray[0], None);
                };
              },
              arrayOfRowValuesList,
            )
            |> Array.to_list;

          Deferred.return(keyAndValueList);
        }
      );
    }
  );
};

let constructPageData = sheetData =>
  sheetData
  >>= (
    sheetData => {
      let surname = List.assoc("$tbxAPP_SURNAME", sheetData);
      let giveName = List.assoc("$tbxAPP_GIVEN_NAME", sheetData);
      let name: DsDataTypes.name = {
        surname:
          switch (surname) {
          | Some(surname) => surname
          | None => raise(Failure("Surname cannot be null"))
          },
        givenName:
          switch (giveName) {
          | Some(givenName) => givenName
          | None => raise(Failure("Given_name cannot be null"))
          },
      };
      print_endline(name.surname);
      print_endline(name.givenName);

      /*****RETURN*******/
      Deferred.return();
    }
  );

let init = (_spec, ()) => {
  let summarySheetData =
    getSheetData("1FM3H6mS1uIYzgvzMERVGLy2ncvUGx82uHT2vof5J1nY");
  let _pageData = constructPageData(summarySheetData);
  Deferred.return();
};
