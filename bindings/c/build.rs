use std::env;

fn main() {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let config =
        cbindgen::Config::from_file("cbindgen.toml").expect("Could not find cbindgen.toml file");

    cbindgen::Builder::new()
        .with_crate(crate_dir)
        .with_config(config)
        .generate()
        .expect("Unable to generate bindings")
        .write_to_file("include/copenpublish.h");
}
