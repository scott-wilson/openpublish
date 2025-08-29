use pyo3::{
    prelude::*,
    types::{PyBool, PyFloat, PyInt, PyString},
};

#[derive(Debug, Clone)]
pub(crate) struct Value {
    inner: base_openpublish::Value,
}

impl<'py> FromPyObject<'py> for Value {
    fn extract_bound(obj: &Bound<'py, PyAny>) -> PyResult<Self> {
        if obj.is_none() {
            Ok(Self {
                inner: base_openpublish::Value::None,
            })
        } else if obj.is_instance_of::<PyBool>() {
            Ok(Self {
                inner: base_openpublish::Value::Boolean(obj.extract::<bool>()?),
            })
        } else if obj.is_instance_of::<PyInt>() {
            Ok(Self {
                inner: base_openpublish::Value::Integer(obj.extract::<i64>()?),
            })
        } else if obj.is_instance_of::<PyFloat>() {
            Ok(Self {
                inner: base_openpublish::Value::Float(obj.extract::<f64>()?),
            })
        } else if obj.is_instance_of::<PyString>() {
            Ok(Self {
                inner: base_openpublish::Value::String(obj.extract::<String>()?),
            })
        } else if let Ok(value) = obj.extract::<std::collections::HashMap<String, Value>>() {
            Ok(Self {
                inner: base_openpublish::Value::Object(
                    value.into_iter().map(|(k, v)| (k, v.inner)).collect(),
                ),
            })
        } else if let Ok(value) = obj.extract::<Vec<Value>>() {
            Ok(Self {
                inner: base_openpublish::Value::Array(value.into_iter().map(|v| v.inner).collect()),
            })
        } else {
            Err(PyErr::new::<pyo3::exceptions::PyTypeError, _>(format!(
                "Cannot convert {} to Value",
                obj.get_type().name()?
            )))
        }
    }
}

impl<'py> IntoPyObject<'py> for Value {
    type Target = PyAny;
    type Output = Bound<'py, Self::Target>; // in most cases this will be `Bound`
    type Error = PyErr; // the conversion error type, has to be convertible to `PyErr`

    fn into_pyobject(self, py: Python<'py>) -> Result<Self::Output, Self::Error> {
        match self.inner {
            base_openpublish::Value::None => py.None().into_pyobject(py).map_err(|err| err.into()),
            base_openpublish::Value::Boolean(value) => value
                .into_pyobject(py)
                .map(|v| v.as_any().clone())
                .map_err(|err| err.into()),
            base_openpublish::Value::Integer(value) => value
                .into_pyobject(py)
                .map(|v| v.into_any())
                .map_err(|err| err.into()),
            base_openpublish::Value::Float(value) => value
                .into_pyobject(py)
                .map(|v| v.into_any())
                .map_err(|err| err.into()),
            base_openpublish::Value::String(value) => value
                .into_pyobject(py)
                .map(|v| v.into_any())
                .map_err(|err| err.into()),
            base_openpublish::Value::Array(value) => value
                .into_iter()
                .map(|v| Value { inner: v })
                .collect::<Vec<Value>>()
                .into_pyobject(py),
            base_openpublish::Value::Object(value) => value
                .into_iter()
                .map(|(k, v)| (k, Value { inner: v }))
                .collect::<std::collections::HashMap<String, Value>>()
                .into_pyobject(py)
                .map(|v| v.into_any()),
        }
    }
}

#[pyclass]
#[derive(Debug, Clone)]
pub(crate) struct Context {
    pub(crate) inner: base_openpublish::Context,
}

impl From<Context> for base_openpublish::Context {
    fn from(value: Context) -> Self {
        value.inner
    }
}

impl From<base_openpublish::Context> for Context {
    fn from(value: base_openpublish::Context) -> Self {
        Self { inner: value }
    }
}

#[pymethods]
impl Context {
    #[new]
    #[pyo3(signature = (value = None))]
    fn new(value: Option<std::collections::HashMap<String, Value>>) -> Self {
        let inner = match value {
            Some(value) => {
                base_openpublish::Context::new(value.into_iter().map(|(k, v)| (k, v.inner)))
            }
            None => base_openpublish::Context::default(),
        };
        Self { inner }
    }

    fn get(&self, key: &str) -> Option<Value> {
        self.inner.get(key).map(|value| Value {
            inner: value.clone(),
        })
    }

    fn set(&mut self, key: &str, value: Value) {
        self.inner.set(key, value.inner);
    }

    fn copy(&self) -> Context {
        self.clone()
    }

    pub(crate) fn to_view(&self) -> ContextView {
        ContextView {
            inner: self.clone(),
        }
    }
}

#[pyclass]
#[derive(Debug, Clone)]
pub(crate) struct ContextView {
    pub(crate) inner: Context,
}

#[pymethods]
impl ContextView {
    fn get(&self, key: &str) -> Option<Value> {
        self.inner.get(key)
    }

    fn copy(&self) -> Context {
        self.inner.clone()
    }

    fn to_view(&self) -> ContextView {
        self.clone()
    }
}
