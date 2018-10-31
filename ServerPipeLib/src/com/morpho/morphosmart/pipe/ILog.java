package com.morpho.morphosmart.pipe;

public interface ILog {
	
	public void log(String message);
	
	public boolean isEnabled();
	
	public void enableLog();
	
	public void disableLog();
	
}
