[%bs.raw {|require('./index.css')|}];

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=ClientApollo.instance>
    <App />
  </ReasonApollo.Provider>,
  "root",
);
