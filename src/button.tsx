import React from './index.ts';

export default function Button({children}: {children: string}) {
  return (
    <gtk-button label={children}/>
  )
}