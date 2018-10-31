package com.morpho.morphosmart.pipe;

public class SimpleLogger implements ILog{
	
	private boolean enableLog;

	@Override
	public void log(String message) {
		if(isEnabled()) {
			System.out.print(message);
		}
	}

	@Override
	public boolean isEnabled() {
		return enableLog;
	}

	@Override
	public void enableLog() {
		enableLog = true;		
	}

	@Override
	public void disableLog() {
		enableLog = false;		
	}

}
