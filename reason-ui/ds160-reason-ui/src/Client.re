let component = ReasonReact.statelessComponent("LegalClient");

let make = (~clientInfo, _children) => {
  ...component,
  render: _self =>
    ReasonReact.(
      <tr>
        <td> {string(clientInfo##name)} </td>
        <td> {string(clientInfo##dataSheet)} </td>
        <td>
          {
            clientInfo##applicationId == "" ?
              <div className="loader-spinner">
                <p className="loader" />
                {string(" Creating...")}
              </div> :
              string(clientInfo##applicationId)
          }
        </td>
        <td> {string(clientInfo##createdAt)} </td>
      </tr>
    ),
};
