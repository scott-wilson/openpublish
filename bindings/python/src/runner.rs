use pyo3::{exceptions::PyRuntimeError, prelude::*};

#[pyfunction]
pub(crate) fn run(py: Python<'_>, publish: Py<PyAny>) -> PyResult<Bound<'_, PyAny>> {
    let wrapper = crate::publish_wrapper::PublishWrapper::new(publish);

    pyo3_async_runtimes::tokio::future_into_py::<_, crate::Context>(py, async move {
        let context = base_openpublish::run(&wrapper)
            .await
            .map_err(|err| PyRuntimeError::new_err(err.to_string()))?;

        Ok(context.into())
    })
}
