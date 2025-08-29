use crate::context::ContextView;
use pyo3::{exceptions::PyNotImplementedError, prelude::*};

#[pyclass(subclass)]
#[derive(Debug)]
pub(crate) struct Publish;

#[pymethods]
impl Publish {
    #[new]
    fn new() -> Self {
        Self
    }

    fn pre_publish<'py>(
        &self,
        py: Python<'py>,
        context: ContextView,
    ) -> PyResult<Bound<'py, PyAny>> {
        pyo3_async_runtimes::tokio::future_into_py(py, async { Ok(context) })
    }

    fn rollback_pre_publish<'py>(
        &self,
        py: Python<'py>,
        #[allow(unused_variables)] context: ContextView,
    ) -> PyResult<Bound<'py, PyAny>> {
        pyo3_async_runtimes::tokio::future_into_py(py, async { Ok(()) })
    }

    fn publish<'py>(
        &self,
        py: Python<'py>,
        #[allow(unused_variables)] context: ContextView,
    ) -> PyResult<Bound<'py, PyAny>> {
        pyo3_async_runtimes::tokio::future_into_py::<_, ()>(py, async {
            Err(PyNotImplementedError::new_err("publish is not implemented"))
        })
    }

    fn rollback_publish<'py>(
        &self,
        py: Python<'py>,
        #[allow(unused_variables)] context: ContextView,
    ) -> PyResult<Bound<'py, PyAny>> {
        pyo3_async_runtimes::tokio::future_into_py(py, async { Ok(()) })
    }

    fn post_publish<'py>(
        &self,
        py: Python<'py>,
        context: ContextView,
    ) -> PyResult<Bound<'py, PyAny>> {
        pyo3_async_runtimes::tokio::future_into_py(py, async { Ok(context) })
    }

    fn rollback_post_publish<'py>(
        &self,
        py: Python<'py>,
        #[allow(unused_variables)] context: ContextView,
    ) -> PyResult<Bound<'py, PyAny>> {
        pyo3_async_runtimes::tokio::future_into_py(py, async { Ok(()) })
    }
}
