struct TestPublish;

#[async_trait::async_trait]
impl openpublish::Publish for TestPublish {
    async fn pre_publish<'a>(
        &self,
        context: &'a openpublish::Context,
    ) -> Result<Option<openpublish::Context>, openpublish::Error> {
        let mut context = context.to_owned();
        context.set("test", openpublish::Value::Integer(1));

        Ok(Some(context))
    }

    async fn publish<'a>(
        &self,
        context: &'a openpublish::Context,
    ) -> Result<Option<openpublish::Context>, openpublish::Error> {
        let mut context = context.to_owned();
        context.set("test", openpublish::Value::Integer(2));

        Ok(Some(context))
    }

    async fn post_publish<'a>(
        &self,
        context: &'a openpublish::Context,
    ) -> Result<Option<openpublish::Context>, openpublish::Error> {
        let mut context = context.to_owned();
        context.set("test", openpublish::Value::Integer(3));

        Ok(Some(context))
    }
}

#[tokio::test]
async fn test_runner_success() {
    let test_publish = TestPublish;

    let result = openpublish::run(&test_publish).await.unwrap();

    assert_eq!(result.get("test").unwrap(), &openpublish::Value::Integer(3));
}
