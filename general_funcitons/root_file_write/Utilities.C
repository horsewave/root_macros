void Utilities_init(void)
{    
    cout << "##############################################################################" << endl;
    cout << "### Utilities_init" << endl;
    cout << "##############################################################################" << endl;

    global_setup  = new GlobalSetup(setupfile);
    utils         = new Utilities(*global_setup);
    stools        = new SiemensTools(utils);
    presto_env    = new PrestoEnvironment(presto_env_setupfile);

}
