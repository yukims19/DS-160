[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
module GetClientTable = [%graphql
  {|
     query findClients{
     clients{
     id
     name
     dataSheet
     applicationId
     createdAt
     updatedAt
     }
     }
     |}
];

module GetClientTableQuery = ReasonApollo.CreateQuery(GetClientTable);
let component = ReasonReact.statelessComponent("ClientTableQuery");

let make = (~updateClientRows, _children) => {
  ...component,
  render: _self =>
    <GetClientTableQuery>
      ...{
           ({result}) =>
             switch (result) {
             | Loading => <div> {"Loading" |> ReasonReact.string} </div>
             | Error(e) => <div> {ReasonReact.string(e##message)} </div>
             | Data(response) =>
               switch (response##clients) {
               | Some(clients) =>
                 /*                 updateClientRows(clients);*/
                 clients
                 |> Array.map(clientInfo =>
                      <Client clientInfo key=clientInfo##id />
                    )
                 |> ReasonReact.array

               | None => <div />
               }
             }
         }
    </GetClientTableQuery>,
};

/*
    module SignIn = [%graphql
   {|
      mutation signIn($email: String!, $password: String!, $rememberMe: Boolean!) {
      signIn(email: $email, password: $password, rememberMe: $rememberMe) {
      success
      user {
  email
      title
      birthday
      }
      }
      }
      |}
 ];

 module SignInMutation = ReasonApollo.CreateMutation(SignIn);

 let make = _children => {
   render: (self) =>
     <SignInMutation>
       ...{
            (signInAction /* Mutation to call */, _) => {
              /* Result of your mutation */

              let signInDetails = SignIn.make(~email="yuki", ());
              <div>
                <button
                  onClick={
                    _mouseEvent =>
                      signInAction(
                        ~variables=signInDetails##variables,
                        ~refetchQueries=[|"getAllPokemons"|],
                        (),
                      )
                      |> Js.Promise.then_(_, (result => self.send(SignIn(result))))
                  }>
                  {ReasonReact.string("Add Pokemon")}
                </button>
              </div>;
            }
          }
     </SignInMutation>,
 };
 */
