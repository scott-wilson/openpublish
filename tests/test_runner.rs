struct TestPublish;

#[async_trait::async_trait]
impl publish::Publish for TestPublish {
    async fn pre_publish<'a>(
        &self,
        context: &'a publish::Context,
    ) -> Result<Option<publish::Context>, publish::Error> {
        let mut context = context.to_owned();
        context.set("test", publish::Value::Integer(1));

        Ok(Some(context))
    }

    async fn publish<'a>(
        &self,
        context: &'a publish::Context,
    ) -> Result<Option<publish::Context>, publish::Error> {
        let mut context = context.to_owned();
        context.set("test", publish::Value::Integer(2));

        Ok(Some(context))
    }

    async fn post_publish<'a>(
        &self,
        context: &'a publish::Context,
    ) -> Result<Option<publish::Context>, publish::Error> {
        let mut context = context.to_owned();
        context.set("test", publish::Value::Integer(3));

        Ok(Some(context))
    }
}

#[tokio::test]
async fn test_runner_success() {
    let test_publish = TestPublish;

    let result = publish::run(&test_publish).await.unwrap();

    assert_eq!(result.get("test").unwrap(), &publish::Value::Integer(3));
}
