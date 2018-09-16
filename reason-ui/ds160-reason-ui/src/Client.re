let component = ReasonReact.statelessComponent("LegalClient");

let make = (~clientInfo: ClientData.client, _children) => {
  ...component,
  render: _self =>
    ReasonReact.(
      <tr>
        <td> {string(clientInfo##name)} </td>
        <td> {string(clientInfo##dataSheet)} </td>
        <td> {string(clientInfo##applicationId)} </td>
        <td> {string(clientInfo##createdAt)} </td>
      </tr>
    ),
};
