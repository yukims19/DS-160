/* TODO: -Get googleSheet id from job
            - fetch OneGraph query of googleSheet
            - talk to Ds-website
            - complete the task
   */
open Async;
open Cohttp;
open Cohttp_async;

let getSheetData = sheetId => {
  print_endline("-----------------------here is the worker-------------");
  let headerList = [
    ("Authentication", "Bearer tQhJQyN55Q3guJou50YSyeY3B8Rn_dXHbs7zglGl8nU"),
    ("Accept", "application/json"),
  ];
  let headers = Cohttp.Header.of_list(headerList);
  let body =
    Cohttp_async.Body.of_string(
      "{\"query\":\"query {\\n  google {\\n    sheets {\\n      sheet(\\n        includeGridData: true\\n        id: \\\""
      ++ sheetId
      ++ "\\\"\\n      ) {\\n        namedRanges {\\n          name\\n          namedRangeId\\n        }\\n        spreadsheetId\\n        spreadsheetUrl\\n        properties {\\n          title\\n          autoRecalc\\n        }\\n        sheets {\\n          data {\\n            rowData {\\n              values {\\n                formattedValue\\n              }\\n            }\\n            startRow\\n          }\\n        }\\n      }\\n    }\\n  }\\n}\\n\",\"variables\":null}",
    );

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
          let dataSheetsArray =
            Yojson.Basic.Util.(
              member("data", bodyJson)
              |> member("google")
              |> member("sheets")
              |> member("sheet")
              |> member("sheets")
              |> to_list
              |> Array.of_list
            );
          /*          print_endline(string_of_int(Array.length(dataSheetsArray)));*/
          let rowDataArray =
            Yojson.Basic.Util.(
              dataSheetsArray[0] |> member("data") |> to_list |> Array.of_list
            );
          let valuesArray =
            Yojson.Basic.Util.(
              rowDataArray[0] |> member("rowData") |> to_list |> Array.of_list
            );
          Array.iter(
            rowValues => print_endline(Yojson.Basic.to_string(rowValues)),
            valuesArray,
          );
          /*          let dataString = Yojson.Basic.to_string(rowDataArray);
                      print_endline("Body" ++ dataString);*/
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

          let surname = List.assoc("$tbxAPP_SURNAME", keyAndValueList);
          switch (surname) {
          | Some(value) => print_endline("$tbxAPP_SURNAME" ++ ":" ++ value)
          | None => print_endline("empty======")
          };
          Deferred.return();
        }
      );
    }
  );
};

let init = (spec, ()) =>
  getSheetData("1FM3H6mS1uIYzgvzMERVGLy2ncvUGx82uHT2vof5J1nY");
