void listAll(string path, const Class* c)  // two-parameter overload
{
	path += c->name();
	cout << path << endl;
	for (int i = 0; i < c->subclasses().size(); i++) {
		listAll(path + "=>", c->subclasses()[i]);
	}
}